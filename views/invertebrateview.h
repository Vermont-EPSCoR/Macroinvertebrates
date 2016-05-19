#ifndef INVERTEBRATEVIEW_H
#define INVERTEBRATEVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QScreen>
#include <QScroller>

#include "../models/invertebrate.h"

void remove(QGridLayout *layout, int row, int column, bool deleteWidgets);
void deleteChildWidgets(QLayoutItem *item);
void removeRow(QGridLayout *layout, int row, bool deleteWidgets);
void removeColumn(QGridLayout *layout, int column, bool deleteWidgets);

namespace Ui {
class InvertebrateView;
}

class InvertebrateView : public QWidget
{
    Q_OBJECT

public:
    explicit InvertebrateView(QWidget *parent = 0);
    ~InvertebrateView();

    void setInfo(const Invertebrate &invertebrate, QString streamName);
    void backButtonClicked();
private:
    Ui::InvertebrateView *ui;
};

#endif // INVERTEBRATEVIEW_H
