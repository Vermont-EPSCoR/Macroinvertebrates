#include "streamlist.h"

StreamModel::StreamModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int StreamModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant StreamModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
