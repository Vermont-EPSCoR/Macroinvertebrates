#ifndef WEBDATASYNCHRONIZER_H
#define WEBDATASYNCHRONIZER_H

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

    QString synchronouslyHeadEtag(const QUrl &url, bool *ok);
    QNetworkReply *synchronouslyGetUrl(const QUrl &url, bool *ok);

    void handleNetworkReplyForStreamList(QNetworkReply* reply);
    void handleNetworkReplyForStreamData(QNetworkReply* reply);
    void handleNetworkReplyForInvertebrateList(QNetworkReply* reply);
    void handleNetworkReplyForInvertebrateData(QNetworkReply* reply);
    void handleNetworkReplyForImageList(QNetworkReply* reply);
    bool handleNetworkReplyForImageData(QNetworkReply* reply, QString localFileName);
public:
    explicit WebDataSynchronizer(QObject *parent = 0);
    ~WebDataSynchronizer();
    void setData(QMutex *mutex, QMap<QString, Invertebrate> *invertebrates, QMap<QString, Stream> *streams);
    void run();

    bool isOk = true;
signals:
    void statusUpdateMessage(const QString &status);
    void finished(WebDataSynchonizerExitStatus status);  // on SUCCEEDED Application should serialize, and optionally notify the user
    void streamSyncComplete();
    void invertebrateSyncComplete();
    void imageSyncComplete();
private slots:
    void syncInvertebrates();
    void syncStreams();
    void syncImages();

    void finalize();
};

#endif // WEBDATASYNCHRONIZER_H
