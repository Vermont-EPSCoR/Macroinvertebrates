#include "invertebrateitemmodel.h"

InvertebrateItemModel::InvertebrateItemModel()
{

}

InvertebrateItemModel::InvertebrateItemModel(const QIcon &icon, const QString &label, const QString &invertebrate_key, QListWidget *parent, int type) :
    QListWidgetItem(icon, label, parent, type)
{
    _key = invertebrate_key;
}

const QString InvertebrateItemModel::key() const
{
    return _key;
}
