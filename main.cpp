#include "application.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    a.performSetUp();
    return a.exec();
}
