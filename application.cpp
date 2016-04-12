#include "application.h"
#include <QEventLoop>

Application::Application(int argc, char *argv[]): QApplication(argc, argv)
{
    listFont = QFont("Times", 20, QFont::Bold);
    homeView.show();
    connect(&homeView, &HomeView::syncAction, this, &Application::transitionHomeToSync);
    connect(&homeView, &HomeView::startButtonClicked, this, &Application::transitionHomeToStream);
    connect(&streamView, &StreamView::backButtonClicked, this, &Application::transitionStreamToHome);

    connect(&homeView, &HomeView::aboutButtonClicked, this, &Application::transitionHomeToAbout);
    connect(&aboutView, &AboutView::backButtonClicked, this, &Application::transitionAboutToHome);

    connect(&streamView, &StreamView::singleStreamDoubleClicked, this, &Application::transitionStreamsToSingleStream);
    connect(&singleStreamView, &SingleStreamView::backButtonClicked, this, &Application::transitionSingleStreamToStreams);

    connect(&singleStreamView, &SingleStreamView::invertebrateDoubleClicked, this, &Application::transitionSingleStreamToInvertebrate);
    connect(&invertebrateView, &InvertebrateView::backButtonClicked, this, &Application::transitionInvertebrateToSingleStream);

    aboutView.setStyleSheet(masterStylesheet);
    syncView.setStyleSheet(masterStylesheet);
    streamView.setStyleSheet(masterStylesheet);
    singleStreamView.setStyleSheet(masterStylesheet);
    invertebrateView.setStyleSheet(masterStylesheet);

    streamView.setListFont(listFont);
    singleStreamView.setListFont(listFont);
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

    streamView.setStreamList(manager.streams.values());
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
    QList<Invertebrate> invertebrates;
    Stream &stream = manager.streams[streamName];
    invertebrates.reserve(stream.invertebrateList.length());

    for(QString &invertebrateName: stream.invertebrateList) {
        Invertebrate invertebrate = manager.invertebrates[invertebrateName];
        if(!invertebrate.name.isNull()) {
            invertebrates.append(invertebrate);
        }
    }

    std::sort(invertebrates.begin(), invertebrates.end());

    singleStreamView.resize(streamView.size());
    singleStreamView.move(streamView.pos());

    singleStreamView.setInfo(invertebrates, streamName);
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

    invertebrateView.setInfo(manager.invertebrates.value(invertebrate), singleStreamView.getStreamName());
    invertebrateView.show();
    singleStreamView.hide();
}

void Application::transitionInvertebrateToSingleStream(const QString &streamName)
{
    singleStreamView.resize(invertebrateView.size());
    singleStreamView.move(invertebrateView.pos());

    //! TODO move to a function
    // Duplicate code
    QList<Invertebrate> invertebrates;
    Stream &stream = manager.streams[streamName];
    invertebrates.reserve(stream.invertebrateList.length());

    for(QString &invertebrateName: stream.invertebrateList) {
        Invertebrate invertebrate = manager.invertebrates[invertebrateName];
        if(!invertebrate.name.isNull()) {
            invertebrates.append(invertebrate);
        }
    }

    std::sort(invertebrates.begin(), invertebrates.end());
    // End duplicate code

    singleStreamView.setInfo(invertebrates, streamName);
    singleStreamView.show();
    invertebrateView.hide();
}
