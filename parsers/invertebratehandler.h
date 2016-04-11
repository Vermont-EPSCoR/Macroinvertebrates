#ifndef INVERTEBRATEHANDLER_H
#define INVERTEBRATEHANDLER_H

#include <QString>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDir>
#include <QDebug>

#include <memory>

#include "../models/invertebrate.h"

class InvertebrateHandler
{
    QString buffer;
    QDir fileSystemHelper;
    QRegularExpression curlyBraceElement;
    QRegularExpression textBlock;

    QString getLocalFileUrl(const QString &remoteFileUrl);
    QString getRemoteFileUrl(const QString &imageName);

    bool localFileIsPresent(const QString &localFileUrl);
public:
    InvertebrateHandler();
    ~InvertebrateHandler();
    void parseInfoboxToInvertebrate(const QString &infoBox, Invertebrate &invertebrate);
    Invertebrate parse(const QString &text);
};

#endif // INVERTEBRATEHANDLER_H
