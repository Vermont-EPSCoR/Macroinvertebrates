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

void HomeView::on_actionSync_triggered()
{
    emit syncAction();
}
