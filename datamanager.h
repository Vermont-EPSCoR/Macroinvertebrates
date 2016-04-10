#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QEventLoop>
#include <QStandardPaths>
#include <QFileSystemModel>
#include <QList>
#include <QNetworkConfiguration>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QMetaObject>
#include <QByteArray>
#include <QXmlStreamReader>

#include <memory>

class DataManager : public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;

    QString imagePath;

    void setupLocalStorageDirectories();
    bool isOk = true;

    QByteArray synchronouslyFetchUrl(const QUrlQuery &query);

public:
    explicit DataManager(QObject *parent = 0);

    void syncStreamsToLocalStorage();
    void syncInvertebratesToLocalStorage();

    void loadInvertebratesFromLocalStorage();
    void loadStreamsFromLocalStorage();
signals:

public slots:
};

#endif // DATAMANAGER_H
