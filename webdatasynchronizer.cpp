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
    if(network.networkAccessible() == QNetworkAccessManager::Accessible) {
        syncStreams();
        syncInvertebrates();
    } else {
        qDebug() << "No network access";
        emit finished(WebDataSynchonizerExitStatus::FAILED_NETWORK_ACCESS);
    }
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

    qDebug() << url;

    QNetworkRequest request(url);
    QNetworkReply *reply = network.get(request);
    connect(reply, &QNetworkReply::finished, [&]() { handleNetworkReplyForStreamList(reply);});
}

void WebDataSynchronizer::handleNetworkReplyForStreamList(QNetworkReply *reply)
{
    reply->deleteLater();
    if(reply->error() != QNetworkReply::NoError || isOk == false) {
        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " isOK: " << isOk;
        isOk = false;
        return;
    }

    QJsonDocument doc = QJsonDocument::fromBinaryData(reply->readAll());

    if(doc.isNull()) {
        qDebug() << "Doc is null/invalid.";
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
    query.addQueryItem("titles", streamTitles.join("|"));
    query.addQueryItem("action", "query");
    query.addQueryItem("export", "");
    query.addQueryItem("exportnowrap", "");
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply *nextReply = network.get(request);
    connect(nextReply, &QNetworkReply::finished, [&]() { handleNetworkReplyForStreamData(nextReply); });
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

    QNetworkRequest request(url);
    QNetworkReply *nextReply = network.get(request);
    connect(nextReply, &QNetworkReply::finished, [&]() {handleNetworkReplyForInvertebrateListing(nextReply); });
}

void WebDataSynchronizer::handleNetworkReplyForInvertebrateListing(QNetworkReply *reply)
{
    reply->deleteLater();
    if(reply->error() != QNetworkReply::NoError || !isOk) {
        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " isOK: " << isOk;
        isOk = false;
        return;
    }

    QJsonDocument doc = QJsonDocument::fromBinaryData(reply->readAll());
    QStringList invertebrateTitles;

    if(doc.isNull()) {
        qDebug() << "Doc is null/invalid in inverts.";
        isOk = false;
        return;
    }

    QJsonArray invertebrateValues = doc.object().value("query").toObject().value("categorymembers").toArray();
    for(const QJsonValue &value: invertebrateValues) {
        invertebrateTitles.append(value.toObject().value("title").toString());
    }

    QUrl url("http://wikieducator.org/api.php");
    QUrlQuery query;
    query.addQueryItem("titles", invertebrateTitles.join("|"));
    query.addQueryItem("action", "query");
    query.addQueryItem("export", "");
    query.addQueryItem("exportnowrap", "");
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply *nextReply = network.get(request);
    connect(nextReply, &QNetworkReply::finished, [&]() { handleNetworkReplyForInvertebrateData(nextReply); });
}

void WebDataSynchronizer::handleNetworkReplyForInvertebrateData(QNetworkReply *reply)
{
    reply->deleteLater();
    if(reply->error() != QNetworkReply::NoError || !isOk) {
        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " isOK: " << isOk;
        isOk = false;
        return;
    }

    QXmlStreamReader xmlReader(reply->readAll());
    InvertebrateHandler handler;

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

    QStringList titles;
    for(Invertebrate &invertebrate: invertebratesFromWeb) {
        titles.append(invertebrate.imageFileRemote);
    }

    QUrl url("http://wikieducator.org/api.php");
    QUrlQuery query;
    query.addQueryItem("action", "query");
    query.addQueryItem("iiurlwidth", "400");
    query.addQueryItem("prop", "imageinfo");
    query.addQueryItem("iiprop", "url");
    query.addQueryItem("format", "json");
    query.addQueryItem("titles", titles.join("|"));
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply *reply = network.get(request);
    connect(reply, &QNetworkReply::finished, [&]() { handleNetworkReplyForImageList(reply); });
}

void WebDataSynchronizer::handleNetworkReplyForImageList(QNetworkReply *reply)
{
    reply->deleteLater();
    if(reply->error() != QNetworkReply::NoError || !isOk) {
        qDebug() << "A network error occurred, or we're not OK: " << reply->errorString() << " isOK: " << isOk;
        isOk = false;
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

    if(doc.isNull()) {
        qDebug() << "Doc is null/invalid in inverts.";
        isOk = false;
        return;
    }

    QMap<QString, Invertebrate*> invertebrateImages;
    for(Invertebrate& invertebrate: invertebratesFromWeb) {
        invertebrateImages.insert(invertebrate.imageFileRemote, &invertebrate);
    }

    QJsonObject images = doc.object().value("query").toObject().value("pages").toObject();
    for(const QJsonValue &value: images) {
        QJsonObject obj =  value.toObject();
        QString title = obj.value("title").toString();

        Invertebrate* invertebrate = invertebrateImages[title];

        if(invertebrate != nullptr) {
            QString extension = title.split(".").last();
            QUrl thumbUrl(obj.value("imageinfo").toArray().at(0).toObject().value("thumburl").toString());
            QString localFileName = QString("%1%2%3.%4").arg(imagePath, directoryHelper.separator(), synchronouslyHeadEtag(thumbUrl).replace("\"",""), extension);

            if(!directoryHelper.exists(localFileName)) {
                imageCounter++;
                QNetworkRequest request(thumbUrl);
                QNetworkReply *nextReply = network.get(request);

                connect(nextReply, &QNetworkReply::finished, [&]() { handleNetworkReplyForImageData(nextReply); });
            } else {
                qDebug() << "Image: " << localFileName << " already exists";
            }
        } else {
            qDebug() << "Unable to load Invertebrate with image: " << title;
        }
    }
}

void WebDataSynchronizer::handleNetworkReplyForImageData(QNetworkReply *reply)
{
    imageCounter--;

    if(reply->error() != QNetworkReply::NoError || reply->bytesAvailable() == 0) {
        if(imageCounter == 0) {
            finalize();
        }
        qDebug() << "Something went wrong in image request: " << reply->url().toString();
        return;
    }

    QString contentType = reply->rawHeader("Content-Type");
    QString extension = "jpg";
    if(contentType.contains("png")) {
        extension = "png";
    }

    QString localFileName = QString("%1%2%3.%4").arg(imagePath, directoryHelper.separator(), reply->rawHeader("ETag").replace("\"",""), extension);
    QFile imageFile(localFileName);
    if(!imageFile.open(QFile::WriteOnly)) {
        qDebug() << "Unable to open image file: " << localFileName;
        return;
    }

    imageFile.write(reply->readAll());
    imageFile.close();
    qDebug() << "Image updated: " << localFileName;

    if(imageCounter == 0) {
        finalize();
    }
}

QString WebDataSynchronizer::synchronouslyHeadEtag(const QUrl &url)
{
    QString etag;

    QNetworkRequest request(url);
    QNetworkAccessManager temporaryManager;
    // Use of a waiter isn't the most reliable method
    QEventLoop waiter;
    auto conn = std::make_shared<QMetaObject::Connection>();
    *conn = connect(&temporaryManager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
        etag.append(reply->rawHeader("ETag"));
        reply->deleteLater();
        QObject::disconnect(*conn);
    });

    connect(&temporaryManager, &QNetworkAccessManager::finished, &waiter, &QEventLoop::quit);
    temporaryManager.get(request);
    waiter.exec();

    return etag;
}

void WebDataSynchronizer::finalize()
{
    if(!isOk) {
        emit finished(WebDataSynchonizerExitStatus::FAILED_RUNTIME);
        return;
    }

    qDebug() << "Finalizing";
    QMutexLocker locker(mutex);
    streamsFromLocal->clear();
    streamsFromLocal->unite(streamsFromWeb);

    invertebratesFromLocal->clear();
    invertebratesFromLocal->unite(invertebratesFromWeb);

    emit finished(WebDataSynchonizerExitStatus::SUCCEEDED);
}

WebDataSynchronizer::~WebDataSynchronizer()
{}
