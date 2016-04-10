#ifndef STREAMMODEL_H
#define STREAMMODEL_H

#include <QAbstractListModel>

class StreamModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit StreamModel(QObject *parent = 0);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // STREAMMODEL_H