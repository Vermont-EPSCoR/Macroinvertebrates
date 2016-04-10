#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QObject>
#include <QDebug>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include "datamanager.h"

#include "views/homeview.h"
#include "views/syncview.h"

class Application : public QApplication
{
    QString masterStylesheet = "background-repeat: no-repeat;"
                                "background-image: url(:/media/background-plain.png);"
                                "background-position: top center;"
                                "background-color: rgb(255, 255, 255);";

    HomeView homeView;
    SyncView syncView;

    DataManager manager;
public:
    Application(int argc, char *argv[]);

public slots:
    void replyReady(QNetworkReply* reply);
    void transitionToSync();
};

#endif // APPLICATION_H
