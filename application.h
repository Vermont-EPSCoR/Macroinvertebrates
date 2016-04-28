#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QObject>
#include <QFont>
#include <QDebug>
#include <QMap>
#include <QString>
#include <QSettings>
#include <QMutex>
#include <QThreadPool>

#include <QNetworkConfigurationManager>

#include <algorithm>

#include "webdatasynchronizer.h"

#include "views/homeview.h"
#include "views/streamview.h"
#include "views/aboutview.h"
#include "views/singlestreamview.h"
#include "views/invertebrateview.h"
#include "views/settingsview.h"

class Application : public QApplication
{
    QString masterStylesheet =  "background-repeat: no-repeat;"
                                "background-image: url(:/media/background-plain.png);"
                                "background-position: top center;"
                                "background-color: rgb(255, 255, 255);";

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

    QString dataPath;
    QString imagePath;

    void setupUiTransitions();
    void transitionWidgets(QWidget *origin, QWidget *destination);
public:
    Application(int argc, char *argv[]);
    ~Application();

public slots:
    void transitionHomeToStream();
    void transitionStreamToHome();

    void transitionAboutToHome();
    void transitionHomeToAbout();

    void transitionStreamsToSingleStream(const QString &streamName);
    void transitionSingleStreamToStreams();

    void transitionSingleStreamToInvertebrate(const QString &invertebrate);
    void transitionInvertebrateToSingleStream(const QString &streamName);

    void transitionHomeToSettings();
    void transitionSettingsToHome();

    void startSync();
    void loadDataFromDisk();
    void saveDataToDisk();
};

#endif // APPLICATION_H
