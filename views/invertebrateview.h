#ifndef INVERTEBRATEVIEW_H
#define INVERTEBRATEVIEW_H

#include <QMainWindow>

#include "../models/invertebrate.h"

namespace Ui {
class InvertebrateView;
}

class InvertebrateView : public QMainWindow
{
    Q_OBJECT
    QString streamName;

public:
    explicit InvertebrateView(QWidget *parent = 0);
    ~InvertebrateView();

    void setInfo(const Invertebrate &invertebrate, QString streamName);

signals:
    void backButtonClicked(const QString& streamName);

private slots:
    void on_pushButton_back_clicked();

private:
    Ui::InvertebrateView *ui;
};

#endif // INVERTEBRATEVIEW_H
