#include "application.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);
//#ifndef ON_IOS
//    QSplashScreen splash(QPixmap(":/media/splash.png"));
//    splash.show();
//    a.processEvents();
//#endif
    a.performSetUp();
//#ifndef ON_IOS
//    splash.finish(a.home());
//#endif
    return a.exec();
}
