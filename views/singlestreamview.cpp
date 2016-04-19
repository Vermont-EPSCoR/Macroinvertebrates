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

void SingleStreamView::setInfo(const QList<Invertebrate> &invertebrates, const QString& streamName)
{
    ui->listWidget->clear();
    ui->listWidget->scrollToTop();
    this->streamName = streamName;

    for(const Invertebrate& invertebrate: invertebrates) {
        QListWidgetItem *item;

        if(invertebrate.imageIsReady) {
            item = new QListWidgetItem(QIcon(invertebrate.imageFileLocal), invertebrate.name);
        } else {
            item = new QListWidgetItem(QIcon(placeHolder), invertebrate.name);
        }

        item->setTextAlignment(Qt::AlignCenter);
        ui->listWidget->addItem(item);
    }
}

void SingleStreamView::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    emit invertebrateDoubleClicked(item->text());
}

const QString &SingleStreamView::getStreamName()
{
    return streamName;
}

void SingleStreamView::setListFont(const QFont &font)
{
    ui->listWidget->setFont(font);
}

void SingleStreamView::on_listWidget_itemClicked(QListWidgetItem *item)
{
    emit invertebrateDoubleClicked(item->text());
}
