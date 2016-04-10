#ifndef SYNCVIEW_H
#define SYNCVIEW_H

#include <QMainWindow>

namespace Ui {
class SyncView;
}

class SyncView : public QMainWindow
{
    Q_OBJECT

public:
    explicit SyncView(QWidget *parent = 0);
    ~SyncView();

private:
    Ui::SyncView *ui;
};

#endif // SYNCVIEW_H
