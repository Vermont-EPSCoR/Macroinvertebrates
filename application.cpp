#include "application.h"

Application::Application(int argc, char *argv[]): QApplication(argc, argv)
{
}

void Application::transitionToAllStreams()
{
    std::vector<QString> streamList;
    QMutexLocker locker(&mutex);
    streamList.reserve(streams.size());
    for(const Stream& stream: streams.values()) {
        if(!stream.title.isEmpty()) {
            streamList.push_back(stream.title);
        }
    }
    locker.unlock();

    std::sort(streamList.begin(), streamList.end());

    StreamView *view = new StreamView(streamList);
    mainWindow.setCentralWidget(view);
    connect(view, &StreamView::backButtonClicked, this, &Application::transitionToHome);
    connect(view, &StreamView::singleStreamClicked, this, &Application::transitionToSingleStream);
    connect(&mainWindow, &MainWindow::backButtonReleased, view, &StreamView::backButtonClicked);
}

void Application::transitionToHome()
{
    HomeView *view = new HomeView();
    mainWindow.setCentralWidget(view);
    connect(view, &HomeView::aboutButtonClicked, this, &Application::transitionToAbout);
    connect(view, &HomeView::syncButtonClicked, this, &Application::transitionToSettings);
    connect(view, &HomeView::startButtonClicked, this, &Application::transitionToAllStreams);
    connect(&mainWindow, &MainWindow::backButtonReleased, view, &HomeView::on_backButton_pressed);
    connect(view, &HomeView::backButtonPressed, [&](){
        quit();
    });
}

void Application::transitionToAbout()
{
    AboutView *view = new AboutView();
    mainWindow.setCentralWidget(view);
    connect(view, &AboutView::backButtonClicked, this, &Application::transitionToHome);
    connect(&mainWindow, &MainWindow::backButtonReleased, view, &AboutView::backButtonClicked);
}

void Application::transitionToSingleStream(const QString &streamName)
{
    std::vector<Invertebrate> invertebratesList;
    QMutexLocker locker(&mutex);
    Stream &stream = streams[streamName];
    invertebratesList.reserve(stream.invertebrateList.length());

    for(QString &invertebrateName: stream.invertebrateList) {
        Invertebrate invertebrate = invertebrates[invertebrateName];
        if(!invertebrate.name.isEmpty()) {
            invertebratesList.push_back(invertebrate);
        }
    }

    locker.unlock();
    std::sort(invertebratesList.begin(), invertebratesList.end());

    SingleStreamView *view = new SingleStreamView(invertebratesList, streamName);
    mainWindow.setCentralWidget(view);
    connect(view, &SingleStreamView::backButtonClicked, this, &Application::transitionToAllStreams);
    connect(view, &SingleStreamView::invertebrateDoubleClicked, this, &Application::transitionToInvertebrate);
    connect(&mainWindow, &MainWindow::backButtonReleased, view, &SingleStreamView::on_backButton_pressed);
}

void Application::transitionToInvertebrate(const QString &invertebrate, const QString &streamName)
{
    QMutexLocker locker(&mutex);
    InvertebrateView *view = new InvertebrateView(invertebrates.value(invertebrate), streamName);
    locker.unlock();

    mainWindow.setCentralWidget(view);
    connect(view, &InvertebrateView::backButtonClicked, this, &Application::transitionToSingleStream);
    connect(&mainWindow, &MainWindow::backButtonReleased, view, &InvertebrateView::on_pushButton_back_pressed);
}

void Application::transitionToSettings()
{
    settings = new SettingsView(syncStatus);
    mainWindow.setCentralWidget(settings);
    connect(settings, &SettingsView::backButtonClicked, this, &Application::transitionToHome);
    connect(settings, &SettingsView::syncButtonClicked, this, &Application::startSync);
}

void Application::loadDataFromDisk()
{
    QDir dir;
    dir.mkpath(imagePath);
    bool needToSync = false;

    loadVariableFromDisk(needToSync, "invertebrate");
    loadVariableFromDisk(needToSync, "stream");

    if(needToSync) {
        stageBundledData();

        loadVariableFromDisk(needToSync, "invertebrate");
        loadVariableFromDisk(needToSync, "stream");

        startSync();
    }
}

void Application::startSync()
{
    // Don't allow multiple syncs to start concurrently
    if(syncStatus == SyncStatus::READY_TO_SYNC) {
        // Let the user choose if they want to sync data on the first run/if data is empty
        QSettings applicationSettings;
        bool firstRunMessageHasBeenShown = applicationSettings.value("firstRunMessageHasBeenShown", false).toBool();

        if(!firstRunMessageHasBeenShown) {
            applicationSettings.setValue("firstRunMessageHasBeenShown", true);
            QMessageBox msgBox;
            msgBox.setText("Welcome new user!");
            msgBox.setInformativeText("This app has been preloaded with some initial data. This data may be out of date and from time to time you should sync with the remote server. Would you like to sync now?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            if(msgBox.exec() == QMessageBox::Cancel) {
                return;
            }
        }

        // Start the sync process
        isSyncingNow = true;
        syncer = new WebDataSynchronizer();
        syncer->setData(&mutex, &invertebrates, &streams);

        // Ensure that quitting the application halts the background process
        connect(this, &Application::aboutToQuit, syncer, &WebDataSynchronizer::stop);
        // Hook up the status updates to the mainWindow's statusBar
        connect(syncer, &WebDataSynchronizer::statusUpdateMessage, this, &Application::syncMessage);
        // Update the settings view when sync is complete
        connect(syncer, &WebDataSynchronizer::destroyed, [&](){
            syncStatus = SyncStatus::READY_TO_SYNC;
            if(!settings.isNull()) {
                settings->toggleSyncButtonText(SyncStatus::READY_TO_SYNC);
            }
        });
        // On start do these things:
        connect(syncer, &WebDataSynchronizer::started, [&](){
            syncStatus = SyncStatus::SYNC_IN_PROGRESS;
            if(!settings.isNull()) {
                settings->toggleSyncButtonText(SyncStatus::SYNC_IN_PROGRESS);
            }
        });

        QThreadPool::globalInstance()->start(syncer);


        // only show the message about syncing running if the user clicked the sync button
        if(!firstRunMessageHasBeenShown) {
            QMessageBox msgBox;
            msgBox.setText("Data syncing has begun!");
            msgBox.setInformativeText("Sync has begun. Items will be updated as they are completed. If you wish to stop, press cancel.");
            msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
            msgBox.setWindowModality(Qt::WindowModal);

            if(msgBox.exec() == QMessageBox::Cancel) {
                stopSync();
            }
        }
    } else if(syncStatus == SyncStatus::SYNC_IN_PROGRESS) {
        // The user requested a data sync when one was already running. They might want to cancel the running sync.
        QMessageBox msgBox;
        msgBox.setText("Data is already syncing. Cancel?");
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setWindowModality(Qt::WindowModal);
        if(msgBox.exec() == QMessageBox::Yes) {
            stopSync();
        }
    } else {
        mainWindow.statusBar()->showMessage("Sync is halting. Stop pressing the button so much.", 10000);
    }
}

Application::~Application() {
}

#ifdef ADD_FS_WATCHER

void Application::reloadStyles()
{
    QFile styles("/Users/morganrodgers/Desktop/MacroinvertebratesV3/styles/app.css");
    if(styles.open(QFile::ReadOnly)) {
        setStyleSheet("/* /");
        QString loadedStyles = styles.readAll();
        qDebug() << loadedStyles;
        setStyleSheet(loadedStyles);
    }
}

#endif

void Application::performSetUp()
{
    setOrganizationDomain("epscor.uvm.edu");
    setOrganizationName("EPSCOR");
    setApplicationName("Macroinvertebrates");

    setStyle("fusion");
    QFile file(":/styles/app.css");
    file.open(QFile::ReadOnly);
    setStyleSheet(file.readAll());
    file.close();

    transitionToHome();
    mainWindow.show();

    dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    imagePath = QString("%1%2%3").arg(dataPath, QDir::separator(), "images");

#ifdef ADD_FS_WATCHER
    watcher.addPath("/Users/morganrodgers/Desktop/MacroinvertebratesV3/styles/app.css");
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &Application::reloadStyles);
#endif

    QSettings settings;
    SyncOptions option = (SyncOptions)settings.value("syncingPreference").toInt();
    loadDataFromDisk();
    if(option != SyncOptions::MANUAL_ONLY) {
        if(option == SyncOptions::ON_STARTUP) {
            startSync();
        }

//        // ELSE TEST FOR WIRELESS
//        QNetworkConfigurationManager ncm;
//        auto nc = ncm.allConfigurations();

//        for (auto &x : nc) {
//            qDebug() << x.name();
//            qDebug() << x.bearerTypeName();
//            qDebug() << x.bearerTypeFamily();
//            qDebug() << x.state();
//            qDebug() << x.state().testFlag(QNetworkConfiguration::Active);
//            qDebug() << x.isValid();

//            if(x.isValid() && x.state().testFlag(QNetworkConfiguration::Active)) {
//                mainWindow->statusBar()->showMessage("We're actively connected to some kind of internets", 10000);
//                qDebug() << "Should be showing";
//                QMessageBox box;
//                box.setText(x.name() + " " + x.bearerTypeName() + " " + x.bearerTypeFamily());
//                box.exec();
//            }
//        }
    }
}

void Application::syncMessage(const QString &message)
{
    if(mutex.tryLock(250)) {
        mainWindow.statusBar()->showMessage(message, 10000);
        mutex.unlock();
    } else {
//        qDebug() << "Unable to lock and set message";
    }
}

void Application::stopSync()
{
    if(syncer) {
        connect(syncer.data(), &WebDataSynchronizer::destroyed, [&]() {
            syncStatus = SyncStatus::READY_TO_SYNC;
            isSyncingNow = false;

            // If we're on the sync view
            if(!settings.isNull()) {
                settings->toggleSyncButtonText(SyncStatus::READY_TO_SYNC);
            }
        });

        syncer->stop();
    } else {
        syncStatus = SyncStatus::READY_TO_SYNC;
        isSyncingNow = false;
    }
}

void Application::loadVariableFromDisk(bool &needToSync, QString variable)
{
    QDir dir;
    QString _data_path = QString("%1%2%3").arg(dataPath, dir.separator(), QString("%1.data").arg(variable));
//    qDebug() << "Looking for data file at: " << _data_path;
    if(!dir.exists(_data_path)) {
//        qDebug() << "File does not exist";
        needToSync = true;
    } else {
//        qDebug() << "Opening existent file";
        QFile dataFile(_data_path);
        if(!dataFile.open(QFile::ReadOnly)) {
//            qDebug() << "Unable to open file";
            return;
        }

        QDataStream loader(&dataFile);
        if(variable == "invertebrate") {
//            qDebug() << "Loading inverts";
            loader >> invertebrates;
        } else if(variable == "stream") {
//            qDebug() << "Loading streams";
            loader >> streams;
        } else {
//            qDebug() << "You asked for a case that doesn't exist. "<< variable;
        }
    }
}

void Application::stageBundledData() {
    QDir dir;
    QString streamDataPath = QString("%1%2%3").arg(dataPath, dir.separator(), "stream.data");
    QString invertebrateDataPath = QString("%1%2%3").arg(dataPath, dir.separator(), "invertebrate.data");

    dir.setPath(":/initial_data/data");
    QFile::copy(":/initial_data/data/stream.data", streamDataPath);
    QFile::copy(":/initial_data/data/invertebrate.data", invertebrateDataPath);

    dir.setPath(":/initial_data/data/images");
    QFileInfoList fileList = dir.entryInfoList();
    for(QFileInfo &entry: fileList) {
        QFile::copy(entry.filePath(), QString("%1%2%3").arg(imagePath, dir.separator(), entry.fileName()));
    }
}
