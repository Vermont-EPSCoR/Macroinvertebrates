#ifndef HOMEVIEW_H
#define HOMEVIEW_H

#include <QMainWindow>

namespace Ui {
class HomeView;
}

class HomeView : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeView(QWidget *parent = 0);
    ~HomeView();

signals:
    void syncAction();
    void startButtonClicked();
    void aboutButtonClicked();

private slots:
    void on_actionSync_triggered();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::HomeView *ui;
};

#endif // HOMEVIEW_H
