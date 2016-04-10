#ifndef STREAM_H
#define STREAM_H

#include <QObject>
#include <QString>

#include "invertebratelist.h"

class Stream : public QObject
{
    Q_OBJECT
public:
    explicit Stream(QObject *parent = 0);
    QString country;
    QString title;
    QString state_or_province;

    double latitude;
    double longitude;

    InvertebrateList invertebrateList;

signals:

public slots:
};

#endif // STREAM_H
