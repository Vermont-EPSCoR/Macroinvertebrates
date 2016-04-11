#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QEventLoop>
#include <QStandardPaths>
#include <QMap>
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
#include <QDir>

#include <memory>

#include "parsers/streamhandler.h"
#include "parsers/invertebratehandler.h"

class DataManager : public QObject
{
    Q_OBJECT

    QNetworkAccessManager manager;
    QDir directoryHelper;
    QString imagePath;
    QString dataPath;
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

    QMap<QString, Stream> streams;
    QMap<QString, Invertebrate> invertebrates;

signals:
    void noLocalDataFound();
    void localInvertebratesLoaded();
    void localStreamsLoaded();
public slots:
};

#endif // DATAMANAGER_H
