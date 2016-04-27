#ifndef ABOUTVIEW_H
#define ABOUTVIEW_H

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class AboutView;
}

class AboutView : public QMainWindow
{
    Q_OBJECT

public:
    explicit AboutView(QWidget *parent = 0);
    ~AboutView();

public slots:
    void updateAbout(const QString& about);

private slots:
    void on_pushButton_clicked();

signals:
    void backButtonClicked();

private:
    Ui::AboutView *ui;
};

#endif // ABOUTVIEW_H
