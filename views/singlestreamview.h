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

public:
    explicit SingleStreamView(QWidget *parent = 0);
    ~SingleStreamView();

    QString placeHolder = "://media/invertebrate-placeholder.jpg";

    void setInvertebrateList(const QList<Invertebrate> &invertebrates);
signals:
    void backButtonClicked();
private slots:
    void on_pushButton_clicked();

private:
    Ui::SingleStreamView *ui;
};

#endif // SINGLESTREAMVIEW_H
