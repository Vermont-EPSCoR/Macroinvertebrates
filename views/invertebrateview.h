#ifndef INVERTEBRATEVIEW_H
#define INVERTEBRATEVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QScreen>
#include <QScroller>
#include <QFrame>

#include "../models/invertebrate.h"

namespace Ui {
class InvertebrateView;
}

class InvertebrateView : public QWidget
{
    Q_OBJECT
    QString originStream;
    void setInfo(const Invertebrate &invertebrate, const QString &streamName);
public:
    explicit InvertebrateView(const Invertebrate &invertebrate, const QString &streamName, QWidget *parent = 0);
    ~InvertebrateView();
signals:
    void backButtonClicked(const QString &originStream);
private slots:
    void on_pushButton_back_pressed();

private:
    Ui::InvertebrateView *ui;
};

#endif // INVERTEBRATEVIEW_H
