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

void StreamView::on_pushButton_clicked()
{
    emit backButtonClicked();
}

void StreamView::setStreamList(const QList<Stream> &streamList)
{
    ui->listWidget->clear();

    for(const Stream& stream: streamList) {
        ui->listWidget->addItem(stream.title);
    }
}

void StreamView::setListFont(const QFont &font)
{
    ui->listWidget->setFont(font);
}

void StreamView::on_listWidget_itemClicked(QListWidgetItem *item)
{
    emit singleStreamDoubleClicked(item->text());
}
