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

void InvertebrateView::setInfo(const Invertebrate &invertebrate, QString streamName)
{
    this->streamName = streamName;
    ui->label_description->setText(invertebrate.description);
    ui->label_family->setText(invertebrate.family);
    ui->label_genus->setText(invertebrate.genus);
    if(invertebrate.imageIsReady) {
        ui->label_image->setPixmap(QPixmap(invertebrate.imageFileLocal));
    } else {
        // Nothing to do because the placeholder is the default image
    }
    ui->label_order->setText(invertebrate.order);
    ui->label_title->setText(invertebrate.name);
}

void InvertebrateView::on_pushButton_back_clicked()
{
    emit backButtonClicked(streamName);
}
