#ifndef INVERTEBRATELIST_H
#define INVERTEBRATELIST_H

#include <QAbstractListModel>

class InvertebrateList : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit InvertebrateList(QObject *parent = 0);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // INVERTEBRATELIST_H
