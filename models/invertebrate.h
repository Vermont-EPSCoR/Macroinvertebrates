#ifndef INVERTEBRATE_H
#define INVERTEBRATE_H

#include <QObject>
#include <QString>

class Invertebrate : public QObject
{
    Q_OBJECT
public:
    explicit Invertebrate(QObject *parent = 0);

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

    bool operator <(const Invertebrate& inv);
    bool operator >(const Invertebrate& inv);
    bool operator ==(const Invertebrate& inv);
signals:

public slots:
};

#endif // INVERTEBRATE_H
