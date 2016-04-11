#include "application.h"
#include <QEventLoop>

Application::Application(int argc, char *argv[]): QApplication(argc, argv)
{
    homeView.show();
    connect(&homeView, &HomeView::syncAction, this, &Application::transitionHomeToSync);
    connect(&homeView, &HomeView::startButtonClicked, this, &Application::transitionHomeToStream);
    connect(&streamView, &StreamView::backButtonClicked, this, &Application::transitionStreamToHome);

    connect(&homeView, &HomeView::aboutButtonClicked, this, &Application::transitionHomeToAbout);
    connect(&aboutView, &AboutView::backButtonClicked, this, &Application::transitionAboutToHome);

    connect(&streamView, &StreamView::singleStreamDoubleClicked, this, &Application::transitionStreamsToSingleStream);
    connect(&singleStreamView, &SingleStreamView::backButtonClicked, this, &Application::transitionSingleStreamToStreams);

    aboutView.setStyleSheet(masterStylesheet);
    syncView.setStyleSheet(masterStylesheet);
    streamView.setStyleSheet(masterStylesheet);
    singleStreamView.setStyleSheet(masterStylesheet);
}

//void Application::replyReady(QNetworkReply *reply)
//{
//    qDebug() << reply->rawHeader("ETag");
//    qDebug() << reply->size();
//    reply->deleteLater();
//}

void Application::transitionHomeToSync()
{
    syncView.show();
    homeView.hide();
}

void Application::transitionHomeToStream()
{
    streamView.setStreamList(manager.streams.values());
    streamView.show();
    homeView.hide();
}

void Application::transitionStreamToHome()
{
    homeView.show();
    streamView.hide();
}

void Application::transitionAboutToHome()
{
    homeView.show();
    aboutView.hide();
}

void Application::transitionHomeToAbout()
{
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

    qDebug() << "Showing " << streamName;
    singleStreamView.setInvertebrateList(invertebrates);
    singleStreamView.show();
    streamView.hide();
}

void Application::transitionSingleStreamToStreams()
{
    streamView.show();
    singleStreamView.hide();
}
