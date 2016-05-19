#ifndef HOMEVIEW_H
#define HOMEVIEW_H

#include <QWidget>

namespace Ui {
class HomeView;
}

class HomeView : public QWidget
{
    Q_OBJECT

public:
    explicit HomeView(QWidget *parent = 0);
    ~HomeView();
signals:
    void syncButtonClicked();
    void startButtonClicked();
    void aboutButtonClicked();
private slots:
    void on_syncButton_pressed();
    void on_startButton_pressed();
    void on_aboutButton_pressed();

private:
    Ui::HomeView *ui;
};

#endif // HOMEVIEW_H
