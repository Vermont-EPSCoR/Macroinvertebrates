#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QObject>
#include <QFont>
#include <QDebug>
#include <QMap>
#include <QString>
#include <QSettings>
#include <QMessageBox>
#include <QMutex>
#include <QThreadPool>
#include <QMessageBox>
#include <QStatusBar>
#include <QSplashScreen>
#include <QMainWindow>

#include <QNetworkConfigurationManager>

#ifdef ADD_FS_WATCHER
#include <QFileSystemWatcher>
#endif

#include <vector>

#include "webdatasynchronizer.h"

#include "views/homeview.h"
#include "views/streamview.h"
#include "views/aboutview.h"
#include "views/singlestreamview.h"
#include "views/invertebrateview.h"
#include "views/settingsview.h"

class Application : public QApplication
{
    WebDataSynchronizer *syncer; // owned by the threadpool's global instance. don't delete
    QMap<QString, Invertebrate> invertebrates;
    QMap<QString, Stream> streams;
    QMutex mutex;
    bool isSyncingNow = false;

    QString dataPath;
    QString imagePath;

    QMainWindow mainWindow;

#ifdef ADD_FS_WATCHER
    QFileSystemWatcher watcher;
#endif
public:
    Application(int argc, char *argv[]);
    void performSetUp();
    QWidget* home();
    ~Application();

private slots:
    void transitionToAllStreams();
    void transitionToHome();
    void transitionToAbout();

    void transitionToSingleStream(const QString &streamName);
    void transitionSingleStreamToStreams();

    void transitionToInvertebrate(const QString &invertebrate, const QString &streamName);
    void transitionInvertebrateToSingleStream();

    void transitionToSettings();
    void transitionSettingsToHome();

    void startSync();
    void stopSync();

    void loadDataFromDisk();
    void saveDataToDisk();

    void syncMessage(const QString& message);
    void updateLastSync();
    void updateAboutText(const QString &about);

#ifdef ADD_FS_WATCHER
    void reloadStyles();
#endif
};

#endif // APPLICATION_H
