#ifndef INVERTEBRATE_H
#define INVERTEBRATE_H

#include <QString>
#include <QDataStream>
#include <QDebug>

class Invertebrate
{

public:
    Invertebrate();

    // Don't sort this
    QString order;
    QString family;
    QString genus;
    QString name;
    QString commonName;

    // Etc. May be sorted
    QString description;
    QString flyName;
    QString imageFileLocal;  // Use ETag
    QString imageFileRemote;

    bool imageIsUpToDate = false;
    bool imageIsReady = false;

    bool operator <(const Invertebrate& inv) const;
    bool operator >(const Invertebrate& inv) const;
    bool operator ==(const Invertebrate& inv) const;
};

QDataStream &operator<<(QDataStream &dataStream, const Invertebrate &invertebrate);
QDataStream &operator>>(QDataStream &dataStream, Invertebrate &invertebrate);
QDebug operator<<(QDebug debug, const Invertebrate &invertebrate);

#endif // INVERTEBRATE_H
