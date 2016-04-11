#include "stream.h"

Stream::Stream()
{

}

QDataStream &operator<<(QDataStream &dataStream, const Stream &stream)
{
    dataStream << stream.country
               << stream.title
               << stream.state_or_province
               << stream.latitude
               << stream.longitude
               << stream.invertebrateList;

    return dataStream;
}

QDataStream &operator>>(QDataStream &dataStream, Stream &stream)
{
    dataStream >> stream.country;
    dataStream >> stream.title;
    dataStream >> stream.state_or_province;
    dataStream >> stream.latitude;
    dataStream >> stream.longitude;
    dataStream >> stream.invertebrateList;

    return dataStream;
}
