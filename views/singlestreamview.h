#ifndef SINGLESTREAMVIEW_H
#define SINGLESTREAMVIEW_H

#include <QWidget>
#include <QListWidgetItem>
#include <QIcon>
#include <QList>
#include <QScroller>
#include <QSettings>
#include <QScreen>
#include <QFile>
#include <QDesktopWidget>
#include <QFontMetrics>

#include <vector>

#include "application.h"
#include "../models/invertebrate.h"

namespace Ui {
class SingleStreamView;
}

class SingleStreamView : public QWidget
{
    Q_OBJECT
    QString streamName;
    QString portrait_css;
public:
    explicit SingleStreamView(const std::vector<Invertebrate> &invertebratesList, const QString &streamName, QWidget *parent = 0);
    ~SingleStreamView();

    void resizeEvent(QResizeEvent *event);
signals:
    void backButtonClicked(const QString& streamName);
    void invertebrateDoubleClicked(const QString &invertebrate, const QString &streamName);
public slots:
    void on_backButton_pressed();
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::SingleStreamView *ui;
};

#endif // SINGLESTREAMVIEW_H
