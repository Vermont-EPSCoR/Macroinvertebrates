#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFont>
#include <QMap>
#include <QMessageBox>
#include <QMessageBox>
#include <QMutex>
#include <QObject>
#include <QPointer>
#include <QSettings>
#include <QSplashScreen>
#include <QStatusBar>
#include <QString>
#include <QThreadPool>

#ifdef ADD_FS_WATCHER
#include <QFileSystemWatcher>
#endif

#include <list>
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
    static const int application_data_version = 9;
    QPointer<WebDataSynchronizer> syncer;
    QMap<QString, Invertebrate> invertebrates;
    QMap<QString, Stream> streams;
    QMutex mutex;
    bool isUpgrade = false;
    SyncStatus syncStatus = SyncStatus::READY_TO_SYNC;

    QString dataPath;
    QString imagePath;

    QPointer<SettingsView> settings;
    MainWindow mainWindow;

    void loadVariableFromDisk(bool &needToSync, QString variable);
    void stageBundledData();

    std::list<int> view_index_stack;

#ifdef ADD_FS_WATCHER
    QFileSystemWatcher watcher;
#endif
public:
    Application(int argc, char *argv[]);
    void performSetUp();
    ~Application();

    int last_view_index();
    void set_last_view_index(int index);

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
