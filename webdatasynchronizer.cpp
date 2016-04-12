#include "webdatasynchronizer.h"

WebDataSynchronizer::WebDataSynchronizer(QObject *parent) : QObject(parent)
{
}

void WebDataSynchronizer::setData(QMutex *mutex, QMap<QString, Invertebrate> *invertebrates, QMap<QString, Stream> *streams)
{
    this->mutex = mutex;
    this->invertebratesFromLocal = invertebrates;
    this->streamsFromLocal = streams;
}

void WebDataSynchronizer::run()
{
    syncStreams();
    syncInvertebrates();
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
    connect(reply, &QNetworkReply::finished, this, &WebDataSynchronizer::handleNetworkReplyForStreamListing);
}

void WebDataSynchronizer::handleNetworkReplyForStreamListing(QNetworkReply *reply)
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
    qSort(streamTitles);

    QUrl url("http://wikieducator.org/api.php");
    QUrlQuery query;
    query.addQueryItem("titles", streamTitles.join("|"));
    query.addQueryItem("action", "query");
    query.addQueryItem("export", "");
    query.addQueryItem("exportnowrap", "");
    url.setQuery(query);

    QNetworkReply *reply = network.get(url);
    connect(reply, &QNetworkReply::finished, this, &WebDataSynchronizer::handleNetworkReplyForStreamData);
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
    QNetworkReply *reply = network.get(request);
    connect(reply, &QNetworkReply::finished, this, &WebDataSynchronizer::handleNetworkReplyForInvertebrateListing);
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
    QNetworkReply *reply = network.get(request);
    connect(reply, &QNetworkReply::finished, this, &WebDataSynchronizer::handleNetworkReplyForInvertebrateData);
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
