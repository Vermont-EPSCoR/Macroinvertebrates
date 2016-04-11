#include "datamanager.h"

DataManager::DataManager(QObject *parent) : QObject(parent)
{
    setupLocalStorageDirectories();
    loadStreamsFromLocalStorage();
    loadInvertebratesFromLocalStorage();
//    sync();
}

void DataManager::sync() {
    // Maybe only do this if we're connected via WiFi so we're not burning someone's data plan without asking?
    if(manager.networkAccessible() == QNetworkAccessManager::Accessible) {
        syncStreamsToLocalStorage();
        syncInvertebratesToLocalStorage();
    } else {
        qDebug() << "Bearer: " << manager.configuration().bearerType();
    }
}

void DataManager::setupLocalStorageDirectories()
{
    bool result;
    // Ensure image directory
    dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    imagePath = QString("%1%2%3").arg(dataPath, directoryHelper.separator(), "images");
    qDebug() << dataPath;
    qDebug() << imagePath;
    result = directoryHelper.mkpath(imagePath);

    if(!result) {
        // Handle errors
        qDebug() << "Failure creating directory";
        isOk = false;
    } else {
        qDebug() << "Success creating directory";
    }
}

void DataManager::syncStreamsToLocalStorage()
{
    // Don't forget the trailing ?
    QUrlQuery streamQuery("http://wikieducator.org/api.php?");
    streamQuery.addQueryItem("action", "query");
    streamQuery.addQueryItem("list", "categorymembers");
    streamQuery.addQueryItem("cmtitle", "Category:Stream");  // Only difference with Invertebrate Query...
    streamQuery.addQueryItem("cmlimit", "500");
    streamQuery.addQueryItem("format", "json");
    streamQuery.addQueryItem("action", "query");
    streamQuery.addQueryItem("cmprop", "ids|title");

    QJsonDocument doc = doc.fromJson(synchronouslyFetchUrl(streamQuery));

    if(doc.isNull()) {
        qDebug() << "Doc is null/invalid.";
        return;
    }

    QJsonArray streamValues = doc.object().value("query").toObject().value("categorymembers").toArray();
    QStringList streamTitles;

    for(const QJsonValue &value: streamValues) {
        streamTitles.append(value.toObject().value("title").toString());
    }

    qSort(streamTitles);

    QUrlQuery streamInvertebratesQuery("http://wikieducator.org/api.php?");
    streamInvertebratesQuery.addQueryItem("titles", streamTitles.join("|"));
    streamInvertebratesQuery.addQueryItem("action", "query");
    streamInvertebratesQuery.addQueryItem("export", "");
    streamInvertebratesQuery.addQueryItem("exportnowrap", "");

    QByteArray result = synchronouslyFetchUrl(streamInvertebratesQuery);
    QXmlStreamReader xmlReader(result);
    StreamHandler handler;

    while(!xmlReader.atEnd()) {
        xmlReader.readNextStartElement();
        if(xmlReader.name() == "text") {
            Stream stream = handler.parse(xmlReader.readElementText());
            streams.insert(stream.title, stream);
        }
    }

    QString streamDataPath = QString("%1%2%3").arg(dataPath, directoryHelper.separator(), "stream.data");
    QFile dataFile(streamDataPath);

    if(!dataFile.open(QFile::WriteOnly)) {
        qDebug() << "Unable to open invertebrates.data";
    } else {
        QDataStream ds(&dataFile);
        ds.setVersion(QDataStream::Qt_5_6);
        ds << streams;
        dataFile.close();
    }

    qDebug() << "STREAMS COMPLETE";
}

void DataManager::syncInvertebratesToLocalStorage()
{
    QUrlQuery invertebrateQuery("http://wikieducator.org/api.php?");
    invertebrateQuery.addQueryItem("action", "query");
    invertebrateQuery.addQueryItem("list", "categorymembers");
    invertebrateQuery.addQueryItem("cmtitle", "Category:Aquatic Invertebrate");  // Only difference with Stream Query...
    invertebrateQuery.addQueryItem("cmlimit", "500");
    invertebrateQuery.addQueryItem("format", "json");
    invertebrateQuery.addQueryItem("action", "query");
    invertebrateQuery.addQueryItem("cmprop", "ids|title");

    QJsonDocument doc = doc.fromJson(synchronouslyFetchUrl(invertebrateQuery));
    QStringList invertebrateTitles;

    if(doc.isNull()) {
        qDebug() << "Doc is null/invalid in inverts.";
    } else {
        QJsonArray invertebrateValues = doc.object().value("query").toObject().value("categorymembers").toArray();
        for(const QJsonValue &value: invertebrateValues) {
            invertebrateTitles.append(value.toObject().value("title").toString());
        }
    }

    QUrlQuery streamInvertebratesQuery("http://wikieducator.org/api.php?");
    streamInvertebratesQuery.addQueryItem("titles", invertebrateTitles.join("|"));
    streamInvertebratesQuery.addQueryItem("action", "query");
    streamInvertebratesQuery.addQueryItem("export", "");
    streamInvertebratesQuery.addQueryItem("exportnowrap", "");

    QByteArray result = synchronouslyFetchUrl(streamInvertebratesQuery);
    QXmlStreamReader xmlReader(result);
    InvertebrateHandler handler;

    while(!xmlReader.atEnd()) {
        xmlReader.readNextStartElement();
        if(xmlReader.name() == "text") {
            Invertebrate invertebrate = handler.parse(xmlReader.readElementText());
            invertebrates.insert(invertebrate.name, invertebrate);
        }
    }

    QString invertebrateDataPath = QString("%1%2%3").arg(dataPath, directoryHelper.separator(), "invertebrates.data");
    QFile dataFile(invertebrateDataPath);
    if(!dataFile.open(QFile::WriteOnly)) {
        qDebug() << "Unable to open invertebrates.data";
    } else {
        QDataStream ds(&dataFile);
        ds.setVersion(QDataStream::Qt_5_6);
        ds << invertebrates;
        dataFile.close();
    }
    qDebug() << "Invertebrates completed";
}

QByteArray DataManager::synchronouslyFetchUrl(const QUrlQuery &query)
{
    QByteArray bytes;

    QNetworkRequest request(query.toString());
    // Use of a waiter isn't the most reliable method
    QEventLoop waiter;
    auto conn = std::make_shared<QMetaObject::Connection>();
    *conn = connect(&manager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
        bytes = reply->readAll();
        reply->deleteLater();
        QObject::disconnect(*conn);
    });

    connect(&manager, &QNetworkAccessManager::finished, &waiter, &QEventLoop::quit);
    manager.get(request);
    waiter.exec();

    return bytes;
}

void DataManager::loadInvertebratesFromLocalStorage()
{
    QString invertebrateDataPath = QString("%1%2%3").arg(dataPath, directoryHelper.separator(), "invertebrates.data");
    if(!directoryHelper.exists(invertebrateDataPath)) {
        emit noLocalDataFound();
        return;
    } else {
        QFile dataFile(invertebrateDataPath);
        if(!dataFile.open(QFile::ReadOnly)) {
            qDebug() << "Unable to open local invertebrate data";
            emit noLocalDataFound();
            return;
        }

        QDataStream loader(&dataFile);
        loader.setVersion(QDataStream::Qt_5_6);
        loader >> invertebrates;
        emit localInvertebratesLoaded();
    }
}

void DataManager::loadStreamsFromLocalStorage()
{
    QString streamDataPath = QString("%1%2%3").arg(dataPath, directoryHelper.separator(), "stream.data");
    if(!directoryHelper.exists(streamDataPath)) {
        emit noLocalDataFound();
        return;
    } else {
        QFile dataFile(streamDataPath);
        if(!dataFile.open(QFile::ReadOnly)) {
            qDebug() << "Unable to open local invertebrate data";
            emit noLocalDataFound();
            return;
        }

        QDataStream loader(&dataFile);
        loader >> streams;

        qDebug() << "Streams count: " << streams.count();
        emit localStreamsLoaded();
    }
}
