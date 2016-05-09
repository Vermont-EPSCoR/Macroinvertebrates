#ifndef STREAMVIEW_H
#define STREAMVIEW_H

#include <QMainWindow>
#include <QList>
#include <QListWidgetItem>
#include <QScroller>

#include "../models/stream.h"

namespace Ui {
class StreamView;
}

class StreamView : public QMainWindow
{
    Q_OBJECT
public:
    explicit StreamView(QWidget *parent = 0);
    ~StreamView();

    void setStreamList(const QList<Stream> &streamList);
    void setListFont(const QFont& font);
private slots:
    void on_pushButton_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_lineEdit_textEdited(const QString &arg1);

signals:
    void backButtonClicked();
    void singleStreamDoubleClicked(const QString& streamName);
private:
    Ui::StreamView *ui;
};

#endif // STREAMVIEW_H
