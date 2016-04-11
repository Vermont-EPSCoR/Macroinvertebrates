#include "singlestreamview.h"
#include "ui_singlestreamview.h"

SingleStreamView::SingleStreamView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SingleStreamView)
{
    ui->setupUi(this);
}

SingleStreamView::~SingleStreamView()
{
    delete ui;
}

void SingleStreamView::on_pushButton_clicked()
{
    emit backButtonClicked();
}

void SingleStreamView::setInvertebrateList(const QList<Invertebrate> &invertebrates)
{
    ui->listWidget->clear();

    for(const Invertebrate& invertebrate: invertebrates) {
        QListWidgetItem *item = new QListWidgetItem(QIcon(placeHolder), invertebrate.name);
        item->setTextAlignment(Qt::AlignCenter);
        ui->listWidget->addItem(item);
    }
}
