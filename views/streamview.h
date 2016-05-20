#ifndef STREAMVIEW_H
#define STREAMVIEW_H

#include <QWidget>
#include <QListWidgetItem>
#include <QScroller>

#include <vector>

#include "../models/stream.h"

namespace Ui {
class StreamView;
}

class StreamView : public QWidget
{
    Q_OBJECT

public:
    explicit StreamView(const std::vector<QString> &streamList, QWidget *parent = 0);
    ~StreamView();
signals:
    void backButtonClicked();
    void singleStreamClicked(const QString &stream);
private slots:
    void on_backButton_pressed();
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_streamSearch_textEdited(const QString &arg1);

private:
    Ui::StreamView *ui;
};

#endif // STREAMVIEW_H
