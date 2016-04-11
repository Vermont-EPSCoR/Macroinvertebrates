#ifndef STREAM_H
#define STREAM_H

#include <QString>
#include <QStringList>
#include <QDataStream>

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

QDataStream &operator<<(QDataStream &dataStream, const Stream &stream);
QDataStream &operator>>(QDataStream &dataStream, Stream &stream);

#endif // STREAM_H
