#ifndef SINGLESTREAMVIEW_H
#define SINGLESTREAMVIEW_H

#include <QWidget>
#include <QListWidgetItem>
#include <QIcon>
#include <QList>
#include <QScroller>

#include "../models/invertebrate.h"

namespace Ui {
class SingleStreamView;
}

class SingleStreamView : public QWidget
{
    Q_OBJECT
    QString streamName;
public:
    explicit SingleStreamView(QWidget *parent = 0);
    ~SingleStreamView();

    void setInfo(const QList<Invertebrate> &invertebrates, const QString &streamName);
    const QString &getStreamName();
signals:
    void backButtonClicked();
    void invertebrateDoubleClicked(const QString &invertebrate);
private slots:
    void on_backButton_pressed();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::SingleStreamView *ui;
};

#endif // SINGLESTREAMVIEW_H
