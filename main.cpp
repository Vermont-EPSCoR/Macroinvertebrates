#include "application.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    QSplashScreen splash(QPixmap(":/media/splash.png"));
    splash.show();
    a.processEvents();
    a.performSetUp();
    splash.finish(a.home());

    return a.exec();
}
