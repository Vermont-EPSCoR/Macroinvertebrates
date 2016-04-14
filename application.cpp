#include "application.h"
#include <QEventLoop>

Application::Application(int argc, char *argv[]): QApplication(argc, argv)
{
    dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    imagePath = QString("%1%2%3").arg(dataPath, QDir::separator(), "images");

    loadDataFromDisk();
    setupUiTransitions();

    QFont listFont("Times", 20);
    homeView.show();

    aboutView.setStyleSheet(masterStylesheet);
    syncView.setStyleSheet(masterStylesheet);
    streamView.setStyleSheet(masterStylesheet);
    singleStreamView.setStyleSheet(masterStylesheet);
    invertebrateView.setStyleSheet(masterStylesheet);

    streamView.setListFont(listFont);
    singleStreamView.setListFont(listFont);

    startSync();
}

void Application::transitionHomeToSync()
{
    syncView.resize(homeView.size());
    syncView.move(homeView.pos());

    syncView.show();
    homeView.hide();
}

void Application::transitionHomeToStream()
{
    streamView.resize(homeView.size());
    streamView.move(homeView.pos());

    streamView.setStreamList(streams.values());
    streamView.show();
    homeView.hide();
}

void Application::transitionStreamToHome()
{
    homeView.resize(streamView.size());
    homeView.move(streamView.pos());

    homeView.show();
    streamView.hide();
}

void Application::transitionAboutToHome()
{
    homeView.resize(aboutView.size());
    homeView.move(aboutView.pos());

    homeView.show();
    aboutView.hide();
}

void Application::transitionHomeToAbout()
{
    aboutView.resize(homeView.size());
    aboutView.move(homeView.pos());

    aboutView.show();
    homeView.hide();
}

void Application::transitionStreamsToSingleStream(const QString &streamName)
{
    QList<Invertebrate> invertebratesList;
    Stream &stream = streams[streamName];
    invertebratesList.reserve(stream.invertebrateList.length());

    for(QString &invertebrateName: stream.invertebrateList) {
        Invertebrate invertebrate = invertebrates[invertebrateName];
        if(!invertebrate.name.isNull()) {
            invertebratesList.append(invertebrate);
        }
    }

    std::sort(invertebratesList.begin(), invertebratesList.end());

    singleStreamView.resize(streamView.size());
    singleStreamView.move(streamView.pos());

    singleStreamView.setInfo(invertebratesList, streamName);
    singleStreamView.show();
    streamView.hide();
}

void Application::transitionSingleStreamToStreams()
{
    streamView.resize(singleStreamView.size());
    streamView.move(singleStreamView.pos());

    streamView.show();
    singleStreamView.hide();
}

void Application::transitionSingleStreamToInvertebrate(const QString &invertebrate)
{
    invertebrateView.resize(singleStreamView.size());
    invertebrateView.move(singleStreamView.pos());

    invertebrateView.setInfo(invertebrates.value(invertebrate), singleStreamView.getStreamName());
    invertebrateView.show();
    singleStreamView.hide();
}

void Application::transitionInvertebrateToSingleStream(const QString &streamName)
{
    singleStreamView.resize(invertebrateView.size());
    singleStreamView.move(invertebrateView.pos());

    //! TODO move to a function
    // Duplicate code
    QList<Invertebrate> invertebratesList;
    Stream &stream = streams[streamName];
    invertebratesList.reserve(stream.invertebrateList.length());

    for(QString &invertebrateName: stream.invertebrateList) {
        Invertebrate invertebrate = invertebrates[invertebrateName];
        if(!invertebrate.name.isNull()) {
            invertebratesList.append(invertebrate);
        }
    }

    std::sort(invertebratesList.begin(), invertebratesList.end());
    // End duplicate code

    singleStreamView.setInfo(invertebratesList, streamName);
    singleStreamView.show();
    invertebrateView.hide();
}

void Application::loadDataFromDisk()
{
    QDir directoryHelper;
    bool result = directoryHelper.mkpath(imagePath);

    QString streamDataPath = QString("%1%2%3").arg(dataPath, directoryHelper.separator(), "stream.data");
    if(!directoryHelper.exists(streamDataPath)) {
//        emit noLocalDataFound();
        return;
    } else {
        QFile dataFile(streamDataPath);
        if(!dataFile.open(QFile::ReadOnly)) {
            qDebug() << "Unable to open local invertebrate data";
//            emit noLocalDataFound();
            return;
        }

        QDataStream loader(&dataFile);
        loader >> streams;
    }

    QString invertebrateDataPath = QString("%1%2%3").arg(dataPath, directoryHelper.separator(), "invertebrate.data");
    if(!directoryHelper.exists(streamDataPath)) {
//        emit noLocalDataFound();
        return;
    } else {
        QFile dataFile(invertebrateDataPath);
        if(!dataFile.open(QFile::ReadOnly)) {
            qDebug() << "Unable to open local invertebrate data";
//            emit noLocalDataFound();
            return;
        }

        QDataStream loader(&dataFile);
        loader >> invertebrates;

        for(const Invertebrate& i: invertebrates) {
            qDebug() << i.name << " : " << i.imageFileLocal;
        }
    }
}

void Application::saveDataToDisk()
{
    qDebug() << streams.count();
    qDebug() << invertebrates.count();

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
    connect(&homeView, &HomeView::syncAction, this, &Application::transitionHomeToSync);
    connect(&homeView, &HomeView::startButtonClicked, this, &Application::transitionHomeToStream);
    connect(&streamView, &StreamView::backButtonClicked, this, &Application::transitionStreamToHome);

    connect(&homeView, &HomeView::aboutButtonClicked, this, &Application::transitionHomeToAbout);
    connect(&aboutView, &AboutView::backButtonClicked, this, &Application::transitionAboutToHome);

    connect(&streamView, &StreamView::singleStreamDoubleClicked, this, &Application::transitionStreamsToSingleStream);
    connect(&singleStreamView, &SingleStreamView::backButtonClicked, this, &Application::transitionSingleStreamToStreams);

    connect(&singleStreamView, &SingleStreamView::invertebrateDoubleClicked, this, &Application::transitionSingleStreamToInvertebrate);
    connect(&invertebrateView, &InvertebrateView::backButtonClicked, this, &Application::transitionInvertebrateToSingleStream);
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
    QThreadPool::globalInstance()->start(syncer);
}
