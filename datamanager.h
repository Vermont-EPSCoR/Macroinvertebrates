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
#include <QThread>

#include <memory>

#include "parsers/streamhandler.h"

class DataManager : public QObject
{
    Q_OBJECT

    QNetworkAccessManager manager;
    QString imagePath;
    bool isOk = true;

    void setupLocalStorageDirectories();
    void syncStreamsToLocalStorage();
    void syncInvertebratesToLocalStorage();
    void loadInvertebratesFromLocalStorage();
    void loadStreamsFromLocalStorage();

    QByteArray synchronouslyFetchUrl(const QUrlQuery &query);

public:
    explicit DataManager(QObject *parent = 0);
    void sync();

signals:

public slots:
};

#endif // DATAMANAGER_H
