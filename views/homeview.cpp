#include "homeview.h"
#include "ui_homeview.h"

HomeView::HomeView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeView)
{
    ui->setupUi(this);
}

HomeView::~HomeView()
{
    delete ui;
}

void HomeView::on_pushButton_clicked()
{
    emit startButtonClicked();
}

void HomeView::on_pushButton_2_clicked()
{
    emit aboutButtonClicked();
}

void HomeView::on_pushButton_3_clicked()
{
    emit syncButtonClicked();
}
