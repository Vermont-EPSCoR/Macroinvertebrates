#include "invertebrateview.h"
#include "ui_invertebrateview.h"

InvertebrateView::InvertebrateView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InvertebrateView)
{
    ui->setupUi(this);
}

InvertebrateView::~InvertebrateView()
{
    delete ui;
}
