#include "aboutview.h"
#include "ui_aboutview.h"

AboutView::AboutView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AboutView)
{
    ui->setupUi(this);
}

AboutView::~AboutView()
{
    delete ui;
}

void AboutView::on_pushButton_clicked()
{
    emit backButtonClicked();
}
