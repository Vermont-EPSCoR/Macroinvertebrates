#include "application.h"

Application::Application(int argc, char *argv[]): QApplication(argc, argv)
{
    setOrganizationDomain("epscor.uvm.edu");
    setOrganizationName("EPSCOR");
    setApplicationName("Macroinvertebrates");

    setStyle("plastique");
    setStyleSheet(masterStylesheet);

    aboutView = new AboutView();
    homeView = new HomeView();
    invertebrateView = new InvertebrateView();
    settingsView = new SettingsView();
    singleStreamView = new SingleStreamView();
    streamView = new StreamView();

    dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    imagePath = QString("%1%2%3").arg(dataPath, QDir::separator(), "images");

    loadDataFromDisk();
    setupUiTransitions();

    QFont listFont("Times", 20);
    homeView->show();

    streamView->setListFont(listFont);
    singleStreamView->setListFont(listFont);

    QSettings settings;
    SyncOptions option = (SyncOptions)settings.value("syncingPreference").toInt();
    if(option != SyncOptions::MANUAL_ONLY) {
        if(option == SyncOptions::ON_STARTUP) {
            startSync();
        }

        // ELSE TEST FOR WIRELESS
        QNetworkConfigurationManager ncm;
        auto nc = ncm.allConfigurations();

        for (auto &x : nc) {
            qDebug() << x.name();
            qDebug() << x.bearerTypeName();
            qDebug() << x.bearerTypeFamily();
            qDebug() << x.state();
            qDebug() << x.state().testFlag(QNetworkConfiguration::Active);
            qDebug() << x.isValid();

            if(x.isValid() && x.state().testFlag(QNetworkConfiguration::Active)) {
                homeView->statusBar()->showMessage("We're actively connected to some kind of internets", 10000);
                qDebug() << "Should be showing";
            }
        }

    }

    connect(settingsView, &SettingsView::syncButtonClicked, this, &Application::startSync);
    connect(settingsView, &SettingsView::reloadStylesClicked, this, &Application::reloadStyles);
}

void Application::transitionHomeToStream()
{
    QMutexLocker locker(&mutex);
    streamView->setStreamList(streams.values());
    locker.unlock();

    transitionWidgets(homeView, streamView);
}

void Application::transitionStreamToHome()
{
    transitionWidgets(streamView, homeView);
}

void Application::transitionAboutToHome()
{
    transitionWidgets(aboutView, homeView);
}

void Application::transitionHomeToAbout()
{
    transitionWidgets(homeView, aboutView);
}

void Application::transitionStreamsToSingleStream(const QString &streamName)
{
    QList<Invertebrate> invertebratesList;
    QMutexLocker locker(&mutex);
    Stream &stream = streams[streamName];
    invertebratesList.reserve(stream.invertebrateList.length());

    for(QString &invertebrateName: stream.invertebrateList) {
        Invertebrate invertebrate = invertebrates[invertebrateName];
        if(!invertebrate.name.isNull()) {
            invertebratesList.append(invertebrate);
        }
    }

    locker.unlock();

    std::sort(invertebratesList.begin(), invertebratesList.end());
    singleStreamView->setInfo(invertebratesList, streamName);

    transitionWidgets(streamView, singleStreamView);
}

void Application::transitionSingleStreamToStreams()
{   
    transitionWidgets(singleStreamView, streamView);
}

void Application::transitionSingleStreamToInvertebrate(const QString &invertebrate)
{
    QMutexLocker locker(&mutex);
    invertebrateView->setInfo(invertebrates.value(invertebrate), singleStreamView->getStreamName());
    locker.unlock();

    transitionWidgets(singleStreamView, invertebrateView);
}

void Application::transitionInvertebrateToSingleStream(const QString &streamName)
{
    transitionWidgets(invertebrateView, singleStreamView);
}

void Application::loadDataFromDisk()
{
    QDir directoryHelper;
    bool result = directoryHelper.mkpath(imagePath);
    bool needToSync = false;

    QString streamDataPath = QString("%1%2%3").arg(dataPath, directoryHelper.separator(), "stream.data");
    if(!directoryHelper.exists(streamDataPath)) {
//        emit noLocalDataFound();
        needToSync = true;
    } else {
        QFile dataFile(streamDataPath);
        if(!dataFile.open(QFile::ReadOnly)) {
            qDebug() << "Unable to open local invertebrate data";
//            emit noLocalDataFound();
            return;
        } else {
            QDataStream loader(&dataFile);
            loader >> streams;
        }
    }

    QString invertebrateDataPath = QString("%1%2%3").arg(dataPath, directoryHelper.separator(), "invertebrate.data");
    if(!directoryHelper.exists(invertebrateDataPath) || needToSync) {
//        emit noLocalDataFound();
        needToSync = true;
    } else {
        QFile dataFile(invertebrateDataPath);
        if(!dataFile.open(QFile::ReadOnly)) {
            qDebug() << "Unable to open local invertebrate data";
//            emit noLocalDataFound();
            return;
        }

        QDataStream loader(&dataFile);
        loader >> invertebrates;
    }

    if(needToSync) {
        qDebug() << "Starting Sync";
        startSync();
    }
}

void Application::saveDataToDisk()
{
    QDir directoryHelper;
    QString streamDataPath = QString("%1%2%3").arg(dataPath, directoryHelper.separator(), "stream.data");
    QFile streamDataFile(streamDataPath);
    if(!streamDataFile.open(QFile::WriteOnly)) {
        qDebug() << "Unable to open local invertebrate data";
        return;
    }

    QDataStream streamSaver(&streamDataFile);
    streamSaver << streams;

    QString invertebrateDataPath = QString("%1%2%3").arg(dataPath, directoryHelper.separator(), "invertebrate.data");
    QFile invertebrateDataFile(invertebrateDataPath);
    if(!invertebrateDataFile.open(QFile::WriteOnly)) {
        qDebug() << "Unable to open local invertebrate data";
        return;
    }

    QDataStream invertebrateSaver(&invertebrateDataFile);
    invertebrateSaver << invertebrates;
}

void Application::setupUiTransitions()
{
    connect(homeView, &HomeView::startButtonClicked, this, &Application::transitionHomeToStream);
    connect(streamView, &StreamView::backButtonClicked, this, &Application::transitionStreamToHome);

    connect(homeView, &HomeView::aboutButtonClicked, this, &Application::transitionHomeToAbout);
    connect(aboutView, &AboutView::backButtonClicked, this, &Application::transitionAboutToHome);

    connect(streamView, &StreamView::singleStreamDoubleClicked, this, &Application::transitionStreamsToSingleStream);
    connect(singleStreamView, &SingleStreamView::backButtonClicked, this, &Application::transitionSingleStreamToStreams);

    connect(singleStreamView, &SingleStreamView::invertebrateDoubleClicked, this, &Application::transitionSingleStreamToInvertebrate);
    connect(invertebrateView, &InvertebrateView::backButtonClicked, this, &Application::transitionInvertebrateToSingleStream);

    connect(homeView, &HomeView::syncButtonClicked, this, &Application::transitionHomeToSettings);
    connect(settingsView, &SettingsView::backButtonClicked, this, &Application::transitionSettingsToHome);
}

void Application::startSync()
{
    syncer = new WebDataSynchronizer();
    syncer->setData(&mutex, &invertebrates, &streams);
    connect(syncer, &WebDataSynchronizer::finished, [&](WebDataSynchonizerExitStatus status) {
        if(status == WebDataSynchonizerExitStatus::SUCCEEDED) {
            saveDataToDisk();
        } else {
            qDebug() << "NOT savingToDisk";
        }
    });

    connect(syncer, &WebDataSynchronizer::aboutStringParsed, aboutView, &AboutView::updateAbout);
    connect(this, &Application::aboutToQuit, syncer, &WebDataSynchronizer::stop);
    QThreadPool::globalInstance()->start(syncer);
    settingsView->updateLastSync();
}

void Application::transitionHomeToSettings()
{
    settingsView->updateLastSync();
    transitionWidgets(homeView, settingsView);
}

void Application::transitionSettingsToHome()
{
    transitionWidgets(settingsView, homeView);
}

void Application::transitionWidgets(QWidget *origin, QWidget *destination)
{
    destination->move(origin->pos());
    destination->resize(origin->size());
    destination->show();
    origin->hide();
}

Application::~Application() {
    aboutView->deleteLater();
    homeView->deleteLater();
    invertebrateView->deleteLater();
    settingsView->deleteLater();
    singleStreamView->deleteLater();
    streamView->deleteLater();
}

void Application::reloadStyles()
{
    QFile styles("/Users/morganrodgers/Desktop/MacroinvertebratesV3/styles/app.css");
    if(styles.open(QFile::ReadOnly)) {
        setStyleSheet("/* /");
        QString loadedStyles = styles.readAll();
        qDebug() << loadedStyles;
        setStyleSheet(loadedStyles);
    } else {
        qDebug() << "Unable to open styles.css";
        setStyleSheet("/* /");
    }
}
