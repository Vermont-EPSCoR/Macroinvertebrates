#ifndef LOCATIONVIEW_H
#define LOCATIONVIEW_H

#include <QMainWindow>

namespace Ui {
class LocationView;
}

class LocationView : public QMainWindow
{
    Q_OBJECT

public:
    explicit LocationView(QWidget *parent = 0);
    ~LocationView();

private:
    Ui::LocationView *ui;
};

#endif // LOCATIONVIEW_H
