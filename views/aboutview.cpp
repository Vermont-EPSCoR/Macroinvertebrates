#include "aboutview.h"
#include "ui_aboutview.h"

AboutView::AboutView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AboutView)
{
    ui->setupUi(this);
    QSettings settings;
    QString about = settings.value("aboutText").toString();

    if(!about.isEmpty()) {
        ui->label->setText(about);
    }
}

AboutView::~AboutView()
{
    delete ui;
}

void AboutView::on_pushButton_clicked()
{
    emit backButtonClicked();
}

void AboutView::updateAbout(const QString &about)
{
    ui->label->setText(about);
}
