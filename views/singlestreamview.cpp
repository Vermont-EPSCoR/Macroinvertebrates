#include "singlestreamview.h"
#include "ui_singlestreamview.h"

SingleStreamView::SingleStreamView(const std::vector<Invertebrate> &invertebratesList, const QString &streamName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleStreamView)
{
    ui->setupUi(this);
    ui->streamNameLabel->setText(streamName);
    this->streamName = streamName;

    // Allow inertial scrolling
    QVariant OvershootPolicy = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
    QScrollerProperties ScrollerProperties = QScroller::scroller(ui->listWidget->viewport())->scrollerProperties();
    ScrollerProperties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, OvershootPolicy);
    ScrollerProperties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, OvershootPolicy);
    QScroller::scroller(ui->listWidget->viewport())->setScrollerProperties(ScrollerProperties);
    QScroller::grabGesture(ui->listWidget->viewport(), QScroller::LeftMouseButtonGesture);


    for(const Invertebrate& invertebrate: invertebratesList) {
        QListWidgetItem *item;

        if(invertebrate.imageIsReady == ImageStatus::READY) {
            QPixmap pixmap(invertebrate.imageFileLocal);
            QSize new_size = pixmap.size() * 0.45;
            item = new QListWidgetItem(QIcon(pixmap.scaled(new_size, Qt::KeepAspectRatio)), invertebrate.name);
        } else if(invertebrate.imageIsReady == ImageStatus::QUEUED_FOR_DOWNLOAD) {
            item = new QListWidgetItem(QIcon(":/media/placeholder-queued.png"), invertebrate.name);
        } else {
            item = new QListWidgetItem(QIcon(":/media/placeholder-unavailable.png"), invertebrate.name);
        }

        item->setTextAlignment(Qt::AlignCenter);
        ui->listWidget->addItem(item);
    }
}

SingleStreamView::~SingleStreamView()
{
    delete ui;
}

void SingleStreamView::on_backButton_pressed()
{
    emit backButtonClicked(streamName);
}

void SingleStreamView::on_listWidget_itemClicked(QListWidgetItem *item)
{
    emit invertebrateDoubleClicked(item->text(), streamName);
}
