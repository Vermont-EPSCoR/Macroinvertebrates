#ifndef INVERTEBRATE_H
#define INVERTEBRATE_H

#include <QString>
#include <QDataStream>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

enum class ImageStatus {
    READY,
    QUEUED_FOR_DOWNLOAD,
    UNAVAILABLE
};

class Invertebrate
{

public:
    static QString imagePath;  // Not serialized
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
    QString imageFileLocal;
    QString imageFileRemote;

    ImageStatus imageIsReady = ImageStatus::QUEUED_FOR_DOWNLOAD;

    bool operator <(const Invertebrate& inv) const;
    bool operator >(const Invertebrate& inv) const;
    bool operator ==(const Invertebrate& inv) const;
};

QDataStream &operator<<(QDataStream &dataStream, const Invertebrate &invertebrate);
QDataStream &operator>>(QDataStream &dataStream, Invertebrate &invertebrate);
QDebug operator<<(QDebug debug, const Invertebrate &invertebrate);

#endif // INVERTEBRATE_H
