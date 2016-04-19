#include "webdatasynchronizer.h"

WebDataSynchronizer::WebDataSynchronizer(QObject *parent) : QObject(parent)
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    imagePath = QString("%1%2%3").arg(dataPath, directoryHelper.separator(), "images");

    QSettings settings;
    lastUpdate = settings.value("lastUpdate").toDateTime();
}

void WebDataSynchronizer::setData(QMutex *mutex, QMap<QString, Invertebrate> *invertebrates, QMap<QString, Stream> *streams)
{
    this->mutex = mutex;
    this->invertebratesFromLocal = invertebrates;
    this->streamsFromLocal = streams;
}

void WebDataSynchronizer::run()
{
    qDebug() << "Running";
    network = new QNetworkAccessManager();
    if(network->networkAccessible() == QNetworkAccessManager::Accessible) {
        syncStreams();
        syncInvertebrates();
        finalize();
    } else {
        qDebug() << "No network access";
        emit finished(WebDataSynchonizerExitStatus::FAILED_NETWORK_ACCESS);
    }

    network->deleteLater();
}

void WebDataSynchronizer::syncStreams()
{
    if(!isOk) {
        qDebug() << "We're not OK";
        return;
    }

    QUrl url("http://wikieducator.org/api.php");
    QUrlQuery query;
    query.addQueryItem("action", "query");
    query.addQueryItem("list", "categorymembers");
    query.addQueryItem("cmtitle", "Category:Stream");  // Only difference with Invertebrate Query...
    query.addQueryItem("cmlimit", "500");
    query.addQueryItem("format", "json");
    query.addQueryItem("action", "query");
    query.addQueryItem("cmprop", "ids|title");
    url.setQuery(query);

    bool ok;
    QNetworkReply *reply = synchronouslyGetUrl(url, &ok);

    if(ok) {
        qDebug() << url;
        handleNetworkReplyForStreamList(reply);
    } else {
        isOk = false;
        reply->deleteLater();
    }
}

void WebDataSynchronizer::handleNetworkReplyForStreamList(QNetworkReply *reply)
{
    if(isOk == false) {
        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " isOK: " << isOk;
        reply->deleteLater();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();

    if(doc.isNull()) {
        qDebug() << "Doc is null/invalid.";
        qDebug() << doc;
        isOk = false;
        return;
    }

    QJsonArray streamValues = doc.object().value("query").toObject().value("categorymembers").toArray();
    QStringList streamTitles;

    for(const QJsonValue &value: streamValues) {
        streamTitles.append(value.toObject().value("title").toString());
    }

    // MediaWiki API recommends sorting so caching is more likely to occur
    std::sort(streamTitles.begin(), streamTitles.end());

    QUrl url("http://wikieducator.org/api.php");
    QUrlQuery query;
    query.addQueryItem("action", "query");
    query.addQueryItem("export", "");
    query.addQueryItem("exportnowrap", "");

    int i = 0;
    bool ok;

    do {
        int batchStart = i * batchSize;
        int batchEnd = batchStart + batchSize;
        QStringList streamTitlesBatch = streamTitles.mid(batchStart, batchEnd);

        query.removeAllQueryItems("titles");
        query.addQueryItem("titles", streamTitlesBatch.join("|"));
        url.setQuery(query);

        QNetworkReply *nextReply = synchronouslyGetUrl(url, &ok);

        if(ok) {
            handleNetworkReplyForStreamData(nextReply);
        } else {
            isOk = false;
            nextReply->deleteLater();
        }

        i++;
    } while((i * batchSize) <= streamTitles.count());
}

void WebDataSynchronizer::handleNetworkReplyForStreamData(QNetworkReply *reply)
{
    reply->deleteLater();
    if(reply->error() != QNetworkReply::NoError || !isOk) {
        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " isOK: " << isOk;
        isOk = false;
        return;
    }

    QXmlStreamReader xmlReader(reply->readAll());
    StreamHandler handler;

    while(!xmlReader.atEnd()) {
        xmlReader.readNextStartElement();
        if(xmlReader.name() == "text") {
            Stream stream = handler.parse(xmlReader.readElementText());
            streamsFromWeb.insert(stream.title, stream);
        }
    }

    emit streamSyncComplete();
}

void WebDataSynchronizer::syncInvertebrates()
{
    qDebug() << "Syncing invertebrates";
    if(!isOk) {
        qDebug() << "We're not OK";
        return;
    }

    QUrl url("http://wikieducator.org/api.php");
    QUrlQuery query;
    query.addQueryItem("action", "query");
    query.addQueryItem("list", "categorymembers");
    query.addQueryItem("cmtitle", "Category:Aquatic Invertebrate");  // Only difference with Stream Query...
    query.addQueryItem("cmlimit", "500");
    query.addQueryItem("format", "json");
    query.addQueryItem("action", "query");
    query.addQueryItem("cmprop", "ids|title");
    url.setQuery(query);

    bool ok;
    QNetworkReply *nextReply = synchronouslyGetUrl(url, &ok);

    if(ok) {
        handleNetworkReplyForInvertebrateList(nextReply);
    } else {
        isOk = false;
        nextReply->deleteLater();
    }
}

void WebDataSynchronizer::handleNetworkReplyForInvertebrateList(QNetworkReply *reply)
{
    qDebug() << "handleNetworkReplyForInvertebrateList";
    if(reply->error() != QNetworkReply::NoError || !isOk) {
        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " isOK: " << isOk;
        reply->deleteLater();
        isOk = false;
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QStringList invertebrateTitles;
    reply->deleteLater();

    if(doc.isNull()) {
        qDebug() << "Doc is null/invalid in inverts.";
        isOk = false;
        return;
    }

    QJsonArray invertebrateValues = doc.object().value("query").toObject().value("categorymembers").toArray();
    for(const QJsonValue &value: invertebrateValues) {
        QString streamName = value.toObject().value("title").toString().trimmed();
        if(!streamName.isEmpty()) {
            invertebrateTitles.append(streamName);
        }
    }

    std::sort(invertebrateTitles.begin(), invertebrateTitles.end());

    QUrl url("http://wikieducator.org/api.php");
    QUrlQuery query;
    query.addQueryItem("action", "query");
    query.addQueryItem("export", "");
    query.addQueryItem("exportnowrap", "");

    int i = 0;
    bool ok;

    do {
        int batchStart = i * batchSize;
        int batchEnd = batchStart + batchSize;
        QStringList invertebrateTitlesBatch = invertebrateTitles.mid(batchStart, batchEnd);

        query.removeAllQueryItems("titles");
        query.addQueryItem("titles", invertebrateTitlesBatch.join("|"));
        url.setQuery(query);
        QNetworkReply *nextReply = synchronouslyGetUrl(url, &ok);

        if(ok) {
            handleNetworkReplyForInvertebrateData(nextReply);
        } else {
            qDebug() << "get FAILED for " << url;
            isOk = false;
            nextReply->deleteLater();
        }

        i++;
    } while((i * batchSize) <= invertebrateTitles.count());
}

void WebDataSynchronizer::handleNetworkReplyForInvertebrateData(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError || !isOk) {
        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " isOK: " << isOk;
        reply->deleteLater();
        isOk = false;
        return;
    }

    QXmlStreamReader xmlReader(reply->readAll());
    InvertebrateHandler handler;
    reply->deleteLater();

    while(!xmlReader.atEnd()) {
        xmlReader.readNextStartElement();
        if(xmlReader.name() == "text") {
            Invertebrate invertebrate = handler.parse(xmlReader.readElementText());
            invertebratesFromWeb.insert(invertebrate.name, invertebrate);
        }
    }

    emit invertebrateSyncComplete();
    syncImages();
}

void WebDataSynchronizer::syncImages()
{
    // if this method is only ever called from the end of the invertebrate sync there's no need to check for isOk
    qDebug() << "syncing images?";
    QStringList titles;
    for(Invertebrate &invertebrate: invertebratesFromWeb) {
        titles.append(invertebrate.imageFileRemote);
    }

    QUrl url("http://wikieducator.org/api.php");
    QUrlQuery query;
    query.addQueryItem("action", "query");
    query.addQueryItem("iiurlwidth", "400");
    query.addQueryItem("prop", "imageinfo");
    query.addQueryItem("iiprop", "url|timestamp|sha1");
    query.addQueryItem("format", "json");
    query.addQueryItem("titles", titles.join("|"));
    url.setQuery(query);

    bool ok;
    QNetworkReply *reply = synchronouslyGetUrl(url, &ok);

    if(ok) {
        handleNetworkReplyForImageList(reply);
    } else {
        isOk = false;
        reply->deleteLater();
    }
}

void WebDataSynchronizer::handleNetworkReplyForImageList(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError || !isOk) {
        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " isOK: " << isOk;
        reply->deleteLater();
        isOk = false;
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();

    if(doc.isNull()) {
        qDebug() << "Doc is null/invalid in ImageList.";
        isOk = false;
        return;
    }

    QMap<QString, Invertebrate*> invertebrateImages;
    for(Invertebrate& invertebrate: invertebratesFromWeb) {
        invertebrateImages.insert(invertebrate.imageFileRemote, &invertebrate);
    }

    QJsonObject images = doc.object().value("query").toObject().value("pages").toObject();
    QCryptographicHash hasher(QCryptographicHash::Sha256);
    bool ok;

    for(const QJsonValue &value: images) {
        QJsonObject obj =  value.toObject();
        QString title = obj.value("title").toString();

        Invertebrate* invertebrate = invertebrateImages[title];

        if(invertebrate != nullptr) {
            qDebug() << "Getting image for " << invertebrate->name;
            QString extension = title.split(".").last().toLower();
            QJsonObject imageInfo = obj.value("imageinfo").toArray().at(0).toObject();
            QUrl thumbUrl(imageInfo.value("thumburl").toString());
            QString sha = imageInfo.value("sha1").toString();

            hasher.reset();
            hasher.addData(title.toUtf8());
            hasher.addData(sha.toUtf8());

            QString localFileName = QString("%1%2%3.%4").arg(imagePath, directoryHelper.separator(), hasher.result().toHex(), extension);

            if(!directoryHelper.exists(localFileName)) {
                QNetworkReply *nextReply = synchronouslyGetUrl(thumbUrl, &ok);

                if(ok) {
                    if(handleNetworkReplyForImageData(nextReply, localFileName)) {
                        invertebrate->imageIsReady = true;
                        invertebrate->imageFileLocal = localFileName;
                    } else {
                        invertebrate->imageIsReady = false;
                    }
                } else {
                    qDebug() << "Request was not ok";
                    invertebrate->imageIsReady = false;
                }
            } else {
                invertebrate->imageIsReady = true;
                invertebrate->imageFileLocal = localFileName;
                qDebug() << "Image: " << localFileName << " already exists";
            }
        } else {
            qDebug() << "Unable to load Invertebrate with image: " << title;
        }
    }
}

bool WebDataSynchronizer::handleNetworkReplyForImageData(QNetworkReply *reply, QString localFileName)
{
    if(reply->error() != QNetworkReply::NoError || reply->bytesAvailable() == 0) {
        qDebug() << "Something went wrong in image request: " << reply->url().toString();
        return false;
    }

    QFile imageFile(localFileName);
    if(!imageFile.open(QFile::WriteOnly)) {
        qDebug() << "Unable to open image file: " << localFileName << " | " << imageFile.errorString();
        return false;
    }

    imageFile.write(reply->readAll());
    imageFile.close();
    reply->deleteLater();
    qDebug() << "Image updated: " << localFileName;

    return true;
}

void WebDataSynchronizer::finalize()
{
    if(!isOk) {
        qDebug() << "Finalizing busted";
        emit finished(WebDataSynchonizerExitStatus::FAILED_RUNTIME);
        return;
    }

    QMutexLocker locker(mutex);
    streamsFromLocal->clear();
    streamsFromLocal->unite(streamsFromWeb);

    invertebratesFromLocal->clear();
    invertebratesFromLocal->unite(invertebratesFromWeb);
    locker.unlock();

    emit finished(WebDataSynchonizerExitStatus::SUCCEEDED);
}

WebDataSynchronizer::~WebDataSynchronizer()
{
}

QNetworkReply *WebDataSynchronizer::synchronouslyGetUrl(const QUrl &url, bool *ok)
{
    QNetworkReply *theReply;
    QNetworkRequest request(url);
    // Use of a waiter isn't the most reliable method, but it does work...
    QEventLoop waiter;
    auto conn = std::make_shared<QMetaObject::Connection>();
    *conn = connect(network, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
        if(reply->error() == QNetworkReply::NoError) {
            *ok = true;
        } else {
            *ok = false;
        }

        theReply = reply;
        QObject::disconnect(*conn);
    });

    connect(network, &QNetworkAccessManager::finished, &waiter, &QEventLoop::quit);
    network->get(request);
    waiter.exec();

    return theReply;
}
