#include "locationview.h"
#include "ui_locationview.h"

LocationView::LocationView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LocationView)
{
    ui->setupUi(this);
}

LocationView::~LocationView()
{
    delete ui;
}
