#include "singlestreamview.h"
#include "ui_singlestreamview.h"

SingleStreamView::SingleStreamView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleStreamView)
{
    ui->setupUi(this);
    QVariant OvershootPolicy = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
    QScrollerProperties ScrollerProperties = QScroller::scroller(ui->listWidget->viewport())->scrollerProperties();
    ScrollerProperties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, OvershootPolicy);
    ScrollerProperties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, OvershootPolicy);
    QScroller::scroller(ui->listWidget->viewport())->setScrollerProperties(ScrollerProperties);
    QScroller::grabGesture(ui->listWidget->viewport(), QScroller::LeftMouseButtonGesture);
}

SingleStreamView::~SingleStreamView()
{
    delete ui;
}

void SingleStreamView::on_backButton_pressed()
{
    emit backButtonClicked();
}

void SingleStreamView::on_listWidget_itemClicked(QListWidgetItem *item)
{
    emit invertebrateDoubleClicked(item->text());
}

void SingleStreamView::setInfo(const QList<Invertebrate> &invertebrates, const QString& streamName)
{
    ui->listWidget->clear();
    ui->listWidget->scrollToTop();
    ui->streamNameLabel->setText(streamName);
    this->streamName = streamName;

    for(const Invertebrate& invertebrate: invertebrates) {
        QListWidgetItem *item;

        if(invertebrate.imageIsReady == ImageStatus::READY) {
            item = new QListWidgetItem(QIcon(invertebrate.imageFileLocal), invertebrate.name);
        } else if(invertebrate.imageIsReady == ImageStatus::QUEUED_FOR_DOWNLOAD) {
            item = new QListWidgetItem(QIcon(":/media/placeholder-queued.png"), invertebrate.name);
        } else {
            item = new QListWidgetItem(QIcon(":/media/placeholder-unavailable.png"), invertebrate.name);
        }

        item->setTextAlignment(Qt::AlignCenter);
        ui->listWidget->addItem(item);
    }
}

const QString &SingleStreamView::getStreamName()
{
    return streamName;
}
