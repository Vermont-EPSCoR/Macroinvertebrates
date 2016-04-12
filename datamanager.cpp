#include "datamanager.h"

DataManager::DataManager(QObject *parent) : QObject(parent)
{
    setupLocalStorageDirectories();
    loadStreamsFromLocalStorage();
    loadInvertebratesFromLocalStorage();
    sync();
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

    QJsonDocument doc = doc.fromJson(synchronouslyGetUrl(QString(streamQuery.toString())));

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

    QByteArray result = synchronouslyGetUrl(streamInvertebratesQuery.toString());
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

    QJsonDocument doc = doc.fromJson(synchronouslyGetUrl(invertebrateQuery.toString()));
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

    QByteArray result = synchronouslyGetUrl(streamInvertebratesQuery.toString());
    QXmlStreamReader xmlReader(result);
    InvertebrateHandler handler;

    while(!xmlReader.atEnd()) {
        xmlReader.readNextStartElement();
        if(xmlReader.name() == "text") {
            Invertebrate invertebrate = handler.parse(xmlReader.readElementText());
            invertebrates.insert(invertebrate.name, invertebrate);
        }
    }

    syncInvertebrateImages();

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

QByteArray DataManager::synchronouslyGetUrl(const QString &query)
{
    QByteArray bytes;
    QNetworkAccessManager temporaryManager;

    QNetworkRequest request(query);
    request.setRawHeader("SetCookieHeader", "type=tmprequest");

    // Use of a waiter isn't the most reliable method, but it does work...
    QEventLoop waiter;
    auto conn = std::make_shared<QMetaObject::Connection>();
    *conn = connect(&temporaryManager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
        bytes = reply->readAll();
        reply->deleteLater();
        QObject::disconnect(*conn);
    });

    connect(&temporaryManager, &QNetworkAccessManager::finished, &waiter, &QEventLoop::quit);
    temporaryManager.get(request);
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

void DataManager::cleanUpOldImages()
{
    //! TODO this stuff
    // Create a set of local image names from the invertebrates
    // iterate over the image directory
    // remove images that are not in the images we want
}

void DataManager::syncInvertebrateImages()
{
    QStringList imageList;
    QMap<QString, Invertebrate*> invertebrateImages;
    imageList.reserve(invertebrates.count());

    for(Invertebrate &invertebrate: invertebrates) {
        imageList.append(invertebrate.imageFileRemote);
        invertebrateImages.insert(invertebrate.imageFileRemote, &invertebrate);
    }

    qSort(imageList);

    QUrlQuery imageQuery("http://wikieducator.org/api.php?action=query&iiurlwidth=400&prop=imageinfo&iiprop=url&format=json&titles=%@");
    imageQuery.addQueryItem("action", "query");
    imageQuery.addQueryItem("iiurlwidth", "400");
    imageQuery.addQueryItem("prop", "imageinfo");
    imageQuery.addQueryItem("iiprop", "url");
    imageQuery.addQueryItem("format", "json");
    imageQuery.addQueryItem("titles", imageList.join("|"));

//    qDebug() << imageList;

    QJsonDocument doc = doc.fromJson(synchronouslyGetUrl(imageQuery.toString()));

    if(doc.isNull()) {
        qDebug() << "Doc is null/invalid in inverts.";
    } else {
        QJsonObject images = doc.object().value("query").toObject().value("pages").toObject();
        for(const QJsonValue &value: images) {
            if(value.toString().contains("File:Mosquito larva.jpg")) {
                qDebug() << value.toString();
            }
            QJsonObject obj =  value.toObject();
            QString title = obj.value("title").toString();
            QString extension = title.split(".").last();
            QString thumbUrl = obj.value("imageinfo").toArray().at(0).toObject().value("thumburl").toString();
            QString localFileName = QString("%1%2%3.%4").arg(imagePath, directoryHelper.separator(), synchronouslyHeadEtag(thumbUrl).replace("\"",""), extension);

            Invertebrate *invertebrate = invertebrateImages[title];

            if(invertebrate == nullptr) {
                // File:Mosquito_larva.jpg does this where the underscore turns into a space
                // that underscore to space behavior isn't universal. how to handle?
                qDebug() << "Title " << title << " results in a nullptr on lookup";
            } else {
                invertebrate->imageFileLocal = localFileName;
                invertebrate->imageFileRemote = thumbUrl;
                if(directoryHelper.exists(localFileName)) {
                    invertebrate->imageIsReady = true;
                    invertebrate->imageIsUpToDate = true;
                } else {
                    invertebrate->imageIsReady = false;
                    invertebrate->imageIsUpToDate = false;
                }
            }
        }
    }

    for(auto &invertebrate: invertebrates) {
        qDebug() << invertebrate.name;
        if(!invertebrate.imageIsReady) {
            qDebug() << invertebrate.imageFileRemote;
            qDebug() << "About to request image";
            QByteArray bytes = synchronouslyGetUrl(invertebrate.imageFileRemote);

            if(!bytes.isEmpty()) {
                qDebug() << "bytes is not empty: " << bytes.length();
                QFile img(invertebrate.imageFileLocal);
                if(img.open(QFile::WriteOnly)) {
                    img.write(bytes);
                    img.close();

                    invertebrate.imageIsReady = true;
                } else {
                    qDebug() << "Unable to write file";
                }
            } else {
                qDebug() << "bytes is empty";
            }
        }
    }
}

QString DataManager::synchronouslyHeadEtag(const QString &url)
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
