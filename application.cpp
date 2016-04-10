#include "application.h"
#include <QEventLoop>

Application::Application(int argc, char *argv[]): QApplication(argc, argv)
{
    connect(&homeView, &HomeView::syncAction, this, &Application::transitionToSync);
    homeView.show();
    syncView.setStyleSheet(masterStylesheet);
}

void Application::replyReady(QNetworkReply *reply)
{
    qDebug() << reply->rawHeader("ETag");
    qDebug() << reply->size();
    reply->deleteLater();
}

void Application::transitionToSync()
{
    syncView.show();
    homeView.hide();
}
