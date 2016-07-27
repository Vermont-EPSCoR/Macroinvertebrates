#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QObject>
#include <QFont>
#include <QDir>
#include <QDebug>
#include <QMap>
#include <QPointer>
#include <QString>
#include <QSettings>
#include <QMessageBox>
#include <QMutex>
#include <QThreadPool>
#include <QMessageBox>
#include <QStatusBar>
#include <QSplashScreen>

#ifdef ADD_FS_WATCHER
#include <QFileSystemWatcher>
#endif

#include <vector>

#include "webdatasynchronizer.h"

#include "views/mainwindow.h"
#include "views/homeview.h"
#include "views/streamview.h"
#include "views/aboutview.h"
#include "views/singlestreamview.h"
#include "views/invertebrateview.h"
#include "views/settingsview.h"

class Application : public QApplication
{
    QPointer<WebDataSynchronizer> syncer;
    QMap<QString, Invertebrate> invertebrates;
    QMap<QString, Stream> streams;
    QMutex mutex;
    bool isSyncingNow = false;
    SyncStatus syncStatus = SyncStatus::READY_TO_SYNC;

    QString dataPath;
    QString imagePath;

    QPointer<SettingsView> settings;
    MainWindow mainWindow;

    void loadVariableFromDisk(bool &needToSync, QString variable);
    void stageBundledData();

#ifdef ADD_FS_WATCHER
    QFileSystemWatcher watcher;
#endif
public:
    Application(int argc, char *argv[]);
    void performSetUp();
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
