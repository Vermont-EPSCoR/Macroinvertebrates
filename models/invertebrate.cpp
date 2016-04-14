#include "invertebrate.h"

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
    ds  << obj.commonName
        << obj.description
        << obj.family
        << obj.flyName
        << obj.genus
        << obj.imageFileLocal
        << obj.imageFileRemote
        << obj.imageIsReady
        << obj.imageIsUpToDate
        << obj.name;

    return ds;
}

QDataStream &operator>>(QDataStream &ds, Invertebrate &obj) {
    ds >> obj.commonName;
    ds >> obj.description;
    ds >> obj.family;
    ds >> obj.flyName;
    ds >> obj.genus;
    ds >> obj.imageFileLocal;
    ds >> obj.imageFileRemote;
    ds >> obj.imageIsReady;
    ds >> obj.imageIsUpToDate;
    ds >> obj.name;

    return ds;
}

QDebug operator<<(QDebug debug, const Invertebrate &invertebrate)
{
    QDebugStateSaver saver(debug);
    QStringList list{invertebrate.name, invertebrate.commonName, invertebrate.description, invertebrate.family, invertebrate.flyName, invertebrate.genus, invertebrate.imageFileLocal, invertebrate.imageFileRemote};
    debug.nospace() << "(" << list.join(", ") << ")";

    return debug;
}
