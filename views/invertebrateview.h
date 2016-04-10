#ifndef INVERTEBRATEVIEW_H
#define INVERTEBRATEVIEW_H

#include <QMainWindow>

namespace Ui {
class InvertebrateView;
}

class InvertebrateView : public QMainWindow
{
    Q_OBJECT

public:
    explicit InvertebrateView(QWidget *parent = 0);
    ~InvertebrateView();

private:
    Ui::InvertebrateView *ui;
};

#endif // INVERTEBRATEVIEW_H
