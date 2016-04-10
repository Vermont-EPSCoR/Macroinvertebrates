#include "streamview.h"
#include "ui_streamview.h"

StreamView::StreamView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StreamView)
{
    ui->setupUi(this);
}

StreamView::~StreamView()
{
    delete ui;
}
