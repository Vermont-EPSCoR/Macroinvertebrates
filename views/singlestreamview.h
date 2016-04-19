#ifndef SINGLESTREAMVIEW_H
#define SINGLESTREAMVIEW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QIcon>
#include <QList>

#include "../models/invertebrate.h"

namespace Ui {
class SingleStreamView;
}

class SingleStreamView : public QMainWindow
{
    Q_OBJECT
    QString streamName;
public:
    explicit SingleStreamView(QWidget *parent = 0);
    ~SingleStreamView();

    QString placeHolder = "://media/invertebrate-placeholder.jpg";

    void setInfo(const QList<Invertebrate> &invertebrates, const QString &streamName);
    void setListFont(const QFont &font);
    const QString &getStreamName();
signals:
    void backButtonClicked();
    void invertebrateDoubleClicked(const QString &invertebrate);
private slots:
    void on_pushButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::SingleStreamView *ui;
};

#endif // SINGLESTREAMVIEW_H
