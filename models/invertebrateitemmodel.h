#ifndef INVERTEBRATEITEMMODEL_H
#define INVERTEBRATEITEMMODEL_H

#include <QListWidgetItem>

class InvertebrateItemModel : public QListWidgetItem
{
    QString _key;
public:
    InvertebrateItemModel();
    InvertebrateItemModel(const QIcon &icon, const QString &label, const QString &invertebrate_key, QListWidget *parent = Q_NULLPTR, int type = Type);
    ~InvertebrateItemModel() {}
    const QString key() const;
};

Q_DECLARE_METATYPE(InvertebrateItemModel)

#endif // INVERTEBRATEITEMMODEL_H
