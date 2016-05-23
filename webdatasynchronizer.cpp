#include "webdatasynchronizer.h"

WebDataSynchronizer::WebDataSynchronizer(QObject *parent) : QObject(parent)
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    imagePath = QString("%1%2%3").arg(dataPath, directoryHelper.separator(), "images");
}

void WebDataSynchronizer::setData(QMutex *mutex, QMap<QString, Invertebrate> *invertebrates, QMap<QString, Stream> *streams)
{
    this->mutex = mutex;
    this->invertebratesFromLocal = invertebrates;
    this->streamsFromLocal = streams;
}

void WebDataSynchronizer::run()
{
    network = QSharedPointer<QNetworkAccessManager>(new QNetworkAccessManager(), [](QNetworkAccessManager *obj){ obj->deleteLater(); });
    if(network->networkAccessible() == QNetworkAccessManager::Accessible) {
//        qDebug() << "Begin";
        syncStreams();
//        qDebug() << "Streams done";
        syncInvertebrates();
//        qDebug() << "Inverts done";
        syncImages();
//        qDebug() << "Images done";
        syncAbout();
//        qDebug() << "About done";

        finalize();
//        qDebug() << "Finalize done";
    } else {
        emit finished(WebDataSynchonizerExitStatus::FAILED_NETWORK_ACCESS);
    }
}

void WebDataSynchronizer::syncStreams()
{
    if(!syncingShouldContinue) {
//        qDebug() << "We're not OK";
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
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> reply(synchronouslyGetUrl(url, &ok));

    if(ok) {
        handleNetworkReplyForStreamList(reply.data());
    } else {
        syncingShouldContinue = false;
    }
}

void WebDataSynchronizer::handleNetworkReplyForStreamList(QNetworkReply *reply)
{
    if(!syncingShouldContinue) {
//        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " syncingShouldContinue: " << syncingShouldContinue;
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

    if(doc.isNull()) {
//        qDebug() << "Doc is null/invalid.";
//        qDebug() << doc;
        syncingShouldContinue = false;
        return;
    }

    QJsonArray streamValues = doc.object().value("query").toObject().value("categorymembers").toArray();
    QStringList streamTitles;

    for(const QJsonValue &value: streamValues) {
        streamTitles.append(value.toObject().value("title").toString());
    }

    // MediaWiki API recommends sorting so caching is more likely to occur, and of course uniqueing
    std::sort(streamTitles.begin(), streamTitles.end());
    std::unique(streamTitles.begin(), streamTitles.end());

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

        QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> nextReply(synchronouslyGetUrl(url, &ok));

        if(ok && syncingShouldContinue) {
            handleNetworkReplyForStreamData(nextReply.data());
            emit statusUpdateMessage(QString("Syncing stream batch %0").arg(i));
        } else {
            syncingShouldContinue = false;
            return;
        }

        i++;
    } while((i * batchSize) <= streamTitles.count());
}

void WebDataSynchronizer::handleNetworkReplyForStreamData(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError || !syncingShouldContinue) {
//        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " syncingShouldContinue: " << syncingShouldContinue;
        syncingShouldContinue = false;
        return;
    }

    QXmlStreamReader xmlReader(reply->readAll());
    StreamHandler handler;

    while(!xmlReader.atEnd()) {
        xmlReader.readNextStartElement();
        if(xmlReader.name() == "text") {
            Stream stream = handler.parse(xmlReader.readElementText());
            if(!stream.title.isEmpty()) {
                QMutexLocker locker(mutex);
                streamsFromLocal->insert(stream.title, stream);
            }
        }
    }

    emit streamSyncComplete();
}

void WebDataSynchronizer::syncInvertebrates()
{
//    qDebug() << "Syncing invertebrates";
    if(!syncingShouldContinue) {
//        qDebug() << "We're not OK";
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
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> nextReply(synchronouslyGetUrl(url, &ok));

    if(ok) {
        handleNetworkReplyForInvertebrateList(nextReply.data());
    } else {
        syncingShouldContinue = false;
    }

    emit invertebrateSyncComplete();
}

void WebDataSynchronizer::handleNetworkReplyForInvertebrateList(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError || !syncingShouldContinue) {
//        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " syncingShouldContinue: " << syncingShouldContinue;
        syncingShouldContinue = false;
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QStringList invertebrateTitles;

    if(doc.isNull()) {
//        qDebug() << "Doc is null/invalid in inverts.";
        syncingShouldContinue = false;
        return;
    }

    QJsonArray invertebrateValues = doc.object().value("query").toObject().value("categorymembers").toArray();
    for(const QJsonValue &value: invertebrateValues) {
        QString streamName = value.toObject().value("title").toString().trimmed();
        if(!streamName.isEmpty()) {
            invertebrateTitles.append(streamName);
        }
    }

    if(!syncingShouldContinue) {
//        qDebug() << "Not ok";
        return;
    }

    std::sort(invertebrateTitles.begin(), invertebrateTitles.end());
    std::unique(invertebrateTitles.begin(), invertebrateTitles.end());

    QUrl url("http://wikieducator.org/api.php");
    QUrlQuery query;
    query.addQueryItem("action", "query");
    query.addQueryItem("export", "");
    query.addQueryItem("exportnowrap", "");

    int i = 0;
    bool ok;

    do {
        emit statusUpdateMessage(QString("Syncing invertebrate batch %0").arg(i));
        int batchStart = i * batchSize;
        int batchEnd = batchStart + batchSize;
        QStringList invertebrateTitlesBatch = invertebrateTitles.mid(batchStart, batchEnd);

        query.removeAllQueryItems("titles");
        query.addQueryItem("titles", invertebrateTitlesBatch.join("|"));
        url.setQuery(query);
        QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> nextReply(synchronouslyGetUrl(url, &ok));

        if(ok && syncingShouldContinue) {
            handleNetworkReplyForInvertebrateData(nextReply.data());
        } else {
//            qDebug() << "get FAILED for " << url;
            syncingShouldContinue = false;
            return;
        }

        i++;
    } while((i * batchSize) <= invertebrateTitles.count());
}

void WebDataSynchronizer::handleNetworkReplyForInvertebrateData(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError || !syncingShouldContinue) {
//        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " syncingShouldContinue: " << syncingShouldContinue;
        syncingShouldContinue = false;
        return;
    }

    QXmlStreamReader xmlReader(reply->readAll());
    InvertebrateHandler handler;

    while(!xmlReader.atEnd()) {
        xmlReader.readNextStartElement();
        if(xmlReader.name() == "text") {
            Invertebrate invertebrate = handler.parse(xmlReader.readElementText());
            QMutexLocker locker(mutex);
            if(invertebrate.name != "Baetis") {  // Baetis is the demo invertebrate and is not useful.
                invertebratesFromLocal->insert(invertebrate.name, invertebrate);
            } else {
                // pass
            }
        }
    }
}

void WebDataSynchronizer::syncImages()
{
    if(!syncingShouldContinue) {
//        qDebug() << "Exiting sync images";
        return;
    }
    // if this method is only ever called from the end of the invertebrate sync there's no need to check for syncingShouldContinue
    QStringList titles;
    for(Invertebrate &invertebrate: *invertebratesFromLocal) {
        titles.append(invertebrate.imageFileRemote);
    }

    std::sort(titles.begin(), titles.end());
    std::unique(titles.begin(), titles.end());

    QUrl url("http://wikieducator.org/api.php");
    QUrlQuery query;
    query.addQueryItem("action", "query");
    query.addQueryItem("iiurlwidth", "400");
    query.addQueryItem("prop", "imageinfo");
    query.addQueryItem("iiprop", "url|timestamp|sha1");
    query.addQueryItem("format", "json");

    int i = 0;
    bool ok;

    QMap<QString, QList<Invertebrate*>> invertebrateImages;
    for(Invertebrate& invertebrate: *invertebratesFromLocal) {
        QString storedFileName = invertebrate.imageFileRemote;
        QString spacesToUnderscores = invertebrate.imageFileRemote.replace(" ", "_");
        QString underscoresToSpaces = invertebrate.imageFileRemote.replace("_", " ");

        if(!invertebrateImages.contains(invertebrate.imageFileRemote)) {
            invertebrateImages.insert(storedFileName, QList<Invertebrate *>());
            // It bothers me that this is necessary, but there is some bad data here
            invertebrateImages.insert(spacesToUnderscores, QList<Invertebrate *>());
            invertebrateImages.insert(underscoresToSpaces, QList<Invertebrate *>());
        }

        invertebrateImages[storedFileName].append(&invertebrate);
        invertebrateImages[spacesToUnderscores].append(&invertebrate);
        invertebrateImages[underscoresToSpaces].append(&invertebrate);
    }

    do {
        emit statusUpdateMessage(QString("Fetching image batch %0").arg(i));
        if(!syncingShouldContinue) {
//            qDebug() << "Exiting sync images";
            return;
        }

        int batchStart = i * batchSize;
        int batchEnd = batchStart + batchSize;

        QStringList imageTitlesBatch = titles.mid(batchStart, batchEnd);

        query.removeAllQueryItems("titles");
        query.addQueryItem("titles", imageTitlesBatch.join("|"));
        url.setQuery(query);

        QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> reply(synchronouslyGetUrl(url, &ok));

        if(ok) {
            handleNetworkReplyForImageList(reply.data(), &invertebrateImages);
        } else {
            syncingShouldContinue = false;
        }

        i++;
    } while((i * batchSize) <= titles.count());
}

void WebDataSynchronizer::handleNetworkReplyForImageList(QNetworkReply *reply, QMap<QString, QList<Invertebrate*>> *invertebrateImages)
{
    if(reply->error() != QNetworkReply::NoError || !syncingShouldContinue) {
//        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " syncingShouldContinue: " << syncingShouldContinue;
        syncingShouldContinue = false;
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

    if(doc.isNull()) {
//        qDebug() << "Doc is null/invalid in ImageList.";
        syncingShouldContinue = false;
        return;
    }

    QJsonObject images = doc.object().value("query").toObject().value("pages").toObject();
    QCryptographicHash hasher(QCryptographicHash::Sha256);
    bool ok;

    for(const QJsonValue &value: images) {
        if(!syncingShouldContinue) {
//            qDebug() << "Exiting from handleNetworkReplyForImageList";
            return;
        }

        QJsonObject obj =  value.toObject();
        QString title = obj.value("title").toString();
        const QList<Invertebrate*> invertebrates = invertebrateImages->value(title);

        for(Invertebrate *invertebrate: invertebrates) {
            QString extension = title.split(".").last().toLower();
            QJsonObject imageInfo = obj.value("imageinfo").toArray().at(0).toObject();
            QString urlString = imageInfo.value("thumburl").toString();

            if(urlString.isEmpty()) {
                continue;
            }

            QUrl thumbUrl(urlString);
            QString sha = imageInfo.value("sha1").toString();

            hasher.reset();
            hasher.addData(title.toUtf8());
            hasher.addData(sha.toUtf8());

            QString localFileName = QString("%1%2%3.%4").arg(imagePath, directoryHelper.separator(), hasher.result().toHex(), extension);

            QMutexLocker locker(mutex);
            if(!directoryHelper.exists(localFileName)) {
                emit statusUpdateMessage(QString("Fetching image for %0").arg(invertebrate->name));
                QNetworkReply *nextReply = synchronouslyGetUrl(thumbUrl, &ok);
                if(ok) {
                    if(handleNetworkReplyForImageData(nextReply, localFileName)) {
                        invertebrate->imageIsReady = ImageStatus::READY;
                        invertebrate->imageFileLocal = localFileName;
                    } else {
                        invertebrate->imageIsReady = ImageStatus::UNAVAILABLE;
                    }
                } else {
//                    qDebug() << "Request was not ok: " << thumbUrl << " " << nextReply->errorString();
                    invertebrate->imageIsReady = ImageStatus::UNAVAILABLE;
                }
            } else {
                invertebrate->imageIsReady = ImageStatus::READY;
                invertebrate->imageFileLocal = localFileName;
            }
        }
    }
}

bool WebDataSynchronizer::handleNetworkReplyForImageData(QNetworkReply *reply, QString localFileName)
{
    if(!syncingShouldContinue) {
        return false;
    }

    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> replyPtr(reply);
    if(reply->error() != QNetworkReply::NoError || reply->bytesAvailable() == 0) {
//        qDebug() << "Something went wrong in image request: " << reply->url().toString();
        return false;
    }

    QFile imageFile(localFileName);
    if(!imageFile.open(QFile::WriteOnly)) {
//        qDebug() << "Unable to open image file: " << localFileName << " | " << imageFile.errorString();
        return false;
    }

    imageFile.write(reply->readAll());
    imageFile.close();

    return true;
}

void WebDataSynchronizer::finalize()
{
//    qDebug() << "Begin finalize";
    if(!syncingShouldContinue) {
//        qDebug() << "emitting finished";
//        emit statusUpdateMessage("Sync stopped; Incomplete.");
        emit finished(WebDataSynchonizerExitStatus::FAILED_RUNTIME);
//        qDebug() << "failure";
        return;
    }

    QSettings settings;
    settings.setValue("lastUpdate", QDateTime::currentDateTime());
//    qDebug() << "last update complete";
//    emit statusUpdateMessage("Syncing completed successfully.");
    emit finished(WebDataSynchonizerExitStatus::SUCCEEDED);
//    qDebug() << "success";
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
    *conn = connect(network.data(), &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
        if(reply->error() == QNetworkReply::NoError) {
            *ok = true;
        } else {
            *ok = false;
        }

        QObject::disconnect(*conn);
    });

    connect(network.data(), &QNetworkAccessManager::finished, &waiter, &QEventLoop::quit);
    theReply = network->get(request);
    connect(this, &WebDataSynchronizer::shouldStop, theReply, &QNetworkReply::abort);
    waiter.exec();

    return theReply;
}

void WebDataSynchronizer::stop()
{
    syncingShouldContinue = false;
    emit shouldStop();
}

void WebDataSynchronizer::handleNetworkReplyForAbout(QNetworkReply* reply)
{
    if(reply->error() != QNetworkReply::NoError || !syncingShouldContinue) {
//        qDebug() << "exiting here";
        return;
    }

    QXmlStreamReader xmlReader(reply->readAll());
    QString htmlStub;

    while(!xmlReader.atEnd()) {
        if(!syncingShouldContinue) {
            return;
        }
        xmlReader.readNextStartElement();
        if(xmlReader.name() == "text") {
            htmlStub = xmlReader.readElementText();
        }
    }

    QGumboDocument doc = QGumboDocument::parse(htmlStub);
    QGumboNodes paragraphs = doc.rootNode().getElementsByTagName(HtmlTag::P);
    QStringList paragraphList;
    for(QGumboNode node: paragraphs) {
        if(!syncingShouldContinue) {
            return;
        }

        QString trimmed = node.innerText().trimmed().replace(QRegularExpression("\\s{2, }"), " ");

        if(!trimmed.isEmpty()) {
            paragraphList << trimmed.prepend('\t');
        }
    }

    if(!paragraphList.isEmpty()) {
        if(!syncingShouldContinue) {
            return;
        }

        QSettings settings;
        settings.setValue("aboutText", paragraphList.join("\n\n"));
        emit aboutStringParsed(paragraphList.join("\n\n"));
    }
}

void WebDataSynchronizer::syncAbout()
{
    if(!syncingShouldContinue) {
        return;
    }

    bool ok;
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> reply(synchronouslyGetUrl(QUrl("http://wikieducator.org/api.php?action=parse&page=AboutStreamsApp&format=xml"), &ok));
    if(ok) {
        handleNetworkReplyForAbout(reply.data());
    }
}
