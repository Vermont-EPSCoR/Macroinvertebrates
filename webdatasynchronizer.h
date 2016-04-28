#ifndef WEBDATASYNCHRONIZER_H
#define WEBDATASYNCHRONIZER_H

#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QEventLoop>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QMetaObject>
#include <QMutex>
#include <QMutexLocker>
#include <QNetworkAccessManager>
#include <QNetworkConfiguration>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QRunnable>
#include <QScopedPointerDeleteLater>
#include <QSettings>
#include <QStandardPaths>
#include <QString>
#include <QThread>
#include <QUrl>
#include <QUrlQuery>
#include <QXmlStreamReader>

#include <algorithm>

#include <qgumbodocument.h>
#include <qgumbonode.h>

#include "models/invertebrate.h"
#include "models/stream.h"

#include "parsers/invertebratehandler.h"
#include "parsers/streamhandler.h"

enum class WebDataSynchonizerExitStatus {
    SUCCEEDED,
    FAILED_RUNTIME,
    FAILED_NETWORK_ACCESS
};

enum class SyncOptions {
    MANUAL_ONLY = 0,
    ON_STARTUP,
    WIFI
};

class WebDataSynchronizer : public QObject, public QRunnable
{
    Q_OBJECT
    int batchSize = 50;

    // Owned by the application instance
    QMutex* mutex = nullptr;
    QMap<QString, Invertebrate> *invertebratesFromLocal = nullptr;
    QMap<QString, Stream> *streamsFromLocal = nullptr;

    // Owned properties
    QEventLoop loop;
    QString imagePath;
    QDir directoryHelper;
    QNetworkAccessManager *network = nullptr;
    QMap<QString, Invertebrate> invertebratesFromWeb;
    QMap<QString, Stream> streamsFromWeb;
    QDateTime lastUpdate;

    QNetworkReply *synchronouslyGetUrl(const QUrl &url, bool *ok);

    void handleNetworkReplyForStreamList(QNetworkReply* reply);
    void handleNetworkReplyForStreamData(QNetworkReply* reply);
    void handleNetworkReplyForInvertebrateList(QNetworkReply* reply);
    void handleNetworkReplyForInvertebrateData(QNetworkReply* reply);
    void handleNetworkReplyForImageList(QNetworkReply* reply, QMap<QString, QList<Invertebrate *>> *invertebrateImages);
    bool handleNetworkReplyForImageData(QNetworkReply* reply, QString localFileName);
    void handleNetworkReplyForAbout(QNetworkReply* reply);
public:
    explicit WebDataSynchronizer(QObject *parent = 0);
    ~WebDataSynchronizer();
    void setData(QMutex *mutex, QMap<QString, Invertebrate> *invertebrates, QMap<QString, Stream> *streams);
    void run();

    bool isOk = true;
public slots:
    void stop();
signals:
    void statusUpdateMessage(const QString &status);
    void finished(WebDataSynchonizerExitStatus status);  // on SUCCEEDED Application should serialize, and optionally notify the user
    void streamSyncComplete();
    void invertebrateSyncComplete();
    void imageSyncComplete();
    void aboutStringParsed(const QString& about);
    void shouldStop();
private slots:
    void syncInvertebrates();
    void syncStreams();
    void syncImages();
    void syncAbout();

    void finalize();
};

#endif // WEBDATASYNCHRONIZER_H
