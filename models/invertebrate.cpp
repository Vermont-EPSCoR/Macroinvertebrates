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
    debug.nospace() << "(\n" <<
                       "    commonName: " << invertebrate.commonName << ",\n" <<
                       "    description: " << invertebrate.description << ",\n" <<
                       "    family: " << invertebrate.family << ",\n" <<
                       "    flyName: " << invertebrate.flyName << ",\n" <<
                       "    genus: " << invertebrate.genus << ",\n" <<
                       "    imageFileLocal: " << invertebrate.imageFileLocal << ",\n" <<
                       "    imageFileRemote: " << invertebrate.imageFileRemote << ",\n" <<
                       "    imageIsReady: " << invertebrate.imageIsReady << ",\n" <<
                       "    imageIsUpToDate: " << invertebrate.imageIsUpToDate << ",\n" <<
                       "    name: " << invertebrate.name << ",\n" <<
                       "    order: " << invertebrate.order <<
                       "\n)";

    return debug;
}
