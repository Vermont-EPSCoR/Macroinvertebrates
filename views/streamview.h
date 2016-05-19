#ifndef STREAMVIEW_H
#define STREAMVIEW_H

#include <QWidget>
#include <QList>
#include <QListWidgetItem>
#include <QScroller>

#include "../models/stream.h"

namespace Ui {
class StreamView;
}

class StreamView : public QWidget
{
    Q_OBJECT

public:
    explicit StreamView(QWidget *parent = 0);
    ~StreamView();
    void StreamView::setStreamList(const QList<Stream> &streamList);
signals:
    void backButtonClicked();
    void singleStreamClicked(const QString &stream);
private slots:
    void on_backButton_pressed();
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::StreamView *ui;
};

#endif // STREAMVIEW_H
