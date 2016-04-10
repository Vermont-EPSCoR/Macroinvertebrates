#include "syncview.h"
#include "ui_syncview.h"

SyncView::SyncView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SyncView)
{
    ui->setupUi(this);
}

SyncView::~SyncView()
{
    delete ui;
}
