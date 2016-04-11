#ifndef INVERTEBRATE_H
#define INVERTEBRATE_H

#include <QString>
#include <QDataStream>

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

    bool operator <(const Invertebrate& inv);
    bool operator >(const Invertebrate& inv);
    bool operator ==(const Invertebrate& inv);

//    QDataStream &operator<<(QDataStream &, const Invertebrate &);
//    QDataStream &operator>>(QDataStream &, Invertebrate &);
};

#endif // INVERTEBRATE_H
