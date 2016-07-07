#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
protected:
    void keyReleaseEvent(QKeyEvent *event);
signals:
    void backButtonReleased();
public slots:
};

#endif // MAINWINDOW_H
