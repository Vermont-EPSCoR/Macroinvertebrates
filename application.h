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

#include <QNetworkConfigurationManager>

#ifdef ADD_FS_WATCHER
#include <QFileSystemWatcher>
#endif

#include "webdatasynchronizer.h"

#include "views/homeview.h"
#include "views/streamview.h"
#include "views/aboutview.h"
#include "views/singlestreamview.h"
#include "views/invertebrateview.h"
#include "views/settingsview.h"

class Application : public QApplication
{
    AboutView *aboutView;
    HomeView *homeView;
    StreamView *streamView;
    SettingsView *settingsView;
    SingleStreamView *singleStreamView;
    InvertebrateView *invertebrateView;

    WebDataSynchronizer *syncer; // owned by the threadpool's global instance. don't delete
    QMap<QString, Invertebrate> invertebrates;
    QMap<QString, Stream> streams;
    QMutex mutex;
    bool isSyncingNow = false;

    QString dataPath;
    QString imagePath;

    QMainWindow *currentView;

#ifdef ADD_FS_WATCHER
    QFileSystemWatcher watcher;
#endif

    void setupUiTransitions();
    void transitionWidgets(QWidget *origin, QWidget *destination);
public:
    Application(int argc, char *argv[]);
    void performSetUp();
    QWidget* home();
    ~Application();

public slots:
    void transitionHomeToStream();
    void transitionStreamToHome();

    void transitionAboutToHome();
    void transitionHomeToAbout();

    void transitionStreamsToSingleStream(const QString &streamName);
    void transitionSingleStreamToStreams();

    void transitionSingleStreamToInvertebrate(const QString &invertebrate);
    void transitionInvertebrateToSingleStream();

    void transitionHomeToSettings();
    void transitionSettingsToHome();

    void startSync();
    void loadDataFromDisk();
    void saveDataToDisk();

    void syncMessage(const QString& message);

#ifdef ADD_FS_WATCHER
    void reloadStyles();
#endif
};

#endif // APPLICATION_H
