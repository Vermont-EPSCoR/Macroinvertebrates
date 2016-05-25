#ifndef ABOUTVIEW_H
#define ABOUTVIEW_H

#include <QWidget>
#include <QSettings>
#include <QScroller>

namespace Ui {
class AboutView;
}

class AboutView : public QWidget
{
    Q_OBJECT

public:
    explicit AboutView(QWidget *parent = 0);
    ~AboutView();
signals:
    void backButtonClicked();
public slots:
    void updateAbout(const QString& about);
private slots:
    void on_backButton_pressed();

private:
    Ui::AboutView *ui;
};

#endif // ABOUTVIEW_H
