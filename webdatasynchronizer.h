#ifndef WEBDATASYNCHRONIZER_H
#define WEBDATASYNCHRONIZER_H

#include <QAtomicInt>
#include <QByteArray>
#include <QDebug>
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
#include <QStandardPaths>
#include <QString>
#include <QThread>
#include <QUrl>
#include <QUrlQuery>
#include <QXmlStreamReader>

#include <algorithm>

#include "models/invertebrate.h"
#include "models/stream.h"

#include "parsers/invertebratehandler.h"
#include "parsers/streamhandler.h"

enum class WebDataSynchonizerExitStatus {
    SUCCEEDED,
    FAILED_RUNTIME,
    FAILED_NETWORK_ACCESS
};

class WebDataSynchronizer : public QObject, public QRunnable
{
    Q_OBJECT
    // Owned by the application instance
    QMutex* mutex;
    QMap<QString, Invertebrate> *invertebratesFromLocal;
    QMap<QString, Stream> *streamsFromLocal;

    // Owned properties
    QString imagePath;
    QDir directoryHelper;
    QNetworkAccessManager *network;
    QAtomicInt imageCounter = 0;
    bool isOk = true;
    QMap<QString, Invertebrate> invertebratesFromWeb;
    QMap<QString, Stream> streamsFromWeb;

    QString synchronouslyHeadEtag(const QUrl &url);
public:
    explicit WebDataSynchronizer(QObject *parent = 0);
    void setData(QMutex *mutex, QMap<QString, Invertebrate> *invertebrates, QMap<QString, Stream> *streams);
    ~WebDataSynchronizer();
    void run();
signals:
    void statusUpdateMessage(const QString &status);
    void finished(WebDataSynchonizerExitStatus status);  // on SUCCEEDED Application should serialize, and optionally notify the user
    void streamSyncComplete();
    void invertebrateSyncComplete();
    void imageSyncComplete();
private slots:
    void handleNetworkReplyForStreamList(QNetworkReply* reply);
    void handleNetworkReplyForStreamData(QNetworkReply* reply);
    void handleNetworkReplyForInvertebrateListing(QNetworkReply* reply);
    void handleNetworkReplyForInvertebrateData(QNetworkReply* reply);
    void handleNetworkReplyForImageList(QNetworkReply* reply);
    void handleNetworkReplyForImageData(QNetworkReply* reply);

    void syncInvertebrates();
    void syncStreams();
    void syncImages();

    void finalize();
};

#endif // WEBDATASYNCHRONIZER_H
