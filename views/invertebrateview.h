#ifndef INVERTEBRATEVIEW_H
#define INVERTEBRATEVIEW_H

#include <QMainWindow>
#include <QGridLayout>

#include "../models/invertebrate.h"

void remove(QGridLayout *layout, int row, int column, bool deleteWidgets);
void deleteChildWidgets(QLayoutItem *item);
void removeRow(QGridLayout *layout, int row, bool deleteWidgets);
void removeColumn(QGridLayout *layout, int column, bool deleteWidgets);

namespace Ui {
class InvertebrateView;
}

class InvertebrateView : public QMainWindow
{
    Q_OBJECT
    QString streamName;

public:
    explicit InvertebrateView(QWidget *parent = 0);
    ~InvertebrateView();

    void setInfo(const Invertebrate &invertebrate, QString streamName);

signals:
    void backButtonClicked(const QString& streamName);

private slots:
    void on_pushButton_back_clicked();

private:
    Ui::InvertebrateView *ui;
};

#endif // INVERTEBRATEVIEW_H
