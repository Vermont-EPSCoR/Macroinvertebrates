#include "invertebrate.h"

QString Invertebrate::imagePath;

Invertebrate::Invertebrate()
{

}

bool Invertebrate::operator <(const Invertebrate& inv) const
{
    return name < inv.name;
}

bool Invertebrate::operator >(const Invertebrate& inv) const
{
    return name > inv.name;
}

bool Invertebrate::operator ==(const Invertebrate& inv) const
{
    return name == inv.name;
}

QDataStream &operator<<(QDataStream &ds, const Invertebrate &obj) {
    if(Invertebrate::imagePath.isEmpty()) {
        QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
        Invertebrate::imagePath = QString("%1%2%3").arg(dataPath, QDir::separator(), "images/");
    }

    QString imageFileLocal = obj.imageFileLocal;
    imageFileLocal.replace(Invertebrate::imagePath, "");
    ds  << obj.commonName
        << obj.description
        << obj.family
        << obj.order
        << obj.flyName
        << obj.genus
        << imageFileLocal
        << obj.imageFileRemote
        << static_cast<int>(obj.imageIsReady)
        << obj.name;

    return ds;
}

QDataStream &operator>>(QDataStream &ds, Invertebrate &obj) {
    if(Invertebrate::imagePath.isEmpty()) {
        QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
        Invertebrate::imagePath = QString("%1%2%3").arg(dataPath, QDir::separator(), "images/");
    }

    ds >> obj.commonName;
    ds >> obj.description;
    ds >> obj.family;
    ds >> obj.order;
    ds >> obj.flyName;
    ds >> obj.genus;
    QString imageFileLocal;
    ds >> imageFileLocal;

    imageFileLocal.prepend(Invertebrate::imagePath);
    obj.imageFileLocal = imageFileLocal;

    ds >> obj.imageFileRemote;
    int imageIsReady;
    ds >> imageIsReady;
    obj.imageIsReady = static_cast<ImageStatus>(imageIsReady);
    ds >> obj.name;

    return ds;
}

QDebug operator<<(QDebug debug, const Invertebrate &invertebrate)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "(\n" <<
                       "    commonName: " << invertebrate.commonName << ",\n" <<
                       "    description: " << invertebrate.description << ",\n" <<
                       "    family: " << invertebrate.family << ",\n" <<
                       "    flyName: " << invertebrate.flyName << ",\n" <<
                       "    genus: " << invertebrate.genus << ",\n" <<
                       "    imageFileLocal: " << invertebrate.imageFileLocal << ",\n" <<
                       "    imageFileRemote: " << invertebrate.imageFileRemote << ",\n" <<
                       "    imageIsReady: " << static_cast<int>(invertebrate.imageIsReady) << ",\n" <<
                       "    name: " << invertebrate.name << ",\n" <<
                       "    order: " << invertebrate.order <<
                       "\n)";

    return debug;
}
