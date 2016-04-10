#ifndef STREAM_H
#define STREAM_H

#include <QString>
#include <QStringList>

#include "invertebratelist.h"

class Stream
{
public:
    Stream();
    QString country;
    QString title;
    QString state_or_province;

    double latitude;
    double longitude;

    QStringList invertebrateList;
};

#endif // STREAM_H
