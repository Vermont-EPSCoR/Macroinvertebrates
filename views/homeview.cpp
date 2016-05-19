#include "homeview.h"
#include "ui_homeview.h"

HomeView::HomeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeView)
{
    ui->setupUi(this);
}

HomeView::~HomeView()
{
    delete ui;
}

void HomeView::on_syncButton_pressed()
{
    emit syncButtonClicked();
}

void HomeView::on_startButton_pressed()
{
    emit startButtonClicked();
}

void HomeView::on_aboutButton_pressed()
{
    emit aboutButtonClicked();
}
