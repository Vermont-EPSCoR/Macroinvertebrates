#include "invertebrateview.h"
#include "ui_invertebrateview.h"

InvertebrateView::InvertebrateView(const Invertebrate &invertebrate, const QString &streamName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InvertebrateView)
{
    ui->setupUi(this);
    QVariant OvershootPolicy = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
    QScrollerProperties ScrollerProperties = QScroller::scroller(ui->scrollArea->viewport())->scrollerProperties();
    ScrollerProperties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, OvershootPolicy);
    ScrollerProperties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, OvershootPolicy);
    QScroller::scroller(ui->scrollArea->viewport())->setScrollerProperties(ScrollerProperties);
    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

    setInfo(invertebrate, streamName);
}

InvertebrateView::~InvertebrateView()
{
    delete ui;
}

void InvertebrateView::setInfo(const Invertebrate &invertebrate, const QString &streamName)
{
#ifndef MOBILE_DEPLOYMENT
    qDebug() << invertebrate;
#endif
    this->originStream = streamName;
    ui->titleLabel->setText(invertebrate.name);

    int row = 0;
    if(!invertebrate.commonName.isEmpty()) {
        ui->gridLayout->addWidget(new QLabel("Common Name:"), row, 0);
        ui->gridLayout->addWidget(new QLabel(invertebrate.commonName), row, 1);
        row++;
        QFrame *hr = new QFrame();
        hr->setFrameShape(QFrame::HLine);
        hr->setFrameShadow(QFrame::Sunken);
        ui->gridLayout->addWidget(hr, row, 0, 1, 2);
        row++;
    }

    if(!invertebrate.family.isEmpty()) {
        ui->gridLayout->addWidget(new QLabel("Family:"), row, 0);
        ui->gridLayout->addWidget(new QLabel(invertebrate.family), row, 1);
        row++;
        QFrame *hr = new QFrame();
        hr->setFrameShape(QFrame::HLine);
        hr->setFrameShadow(QFrame::Sunken);
        ui->gridLayout->addWidget(hr, row, 0, 1, 2);
        row++;
    }

    if(!invertebrate.genus.isEmpty()) {
        ui->gridLayout->addWidget(new QLabel("Genus:"), row, 0);
        ui->gridLayout->addWidget(new QLabel(invertebrate.genus), row, 1);
        row++;
        QFrame *hr = new QFrame();
        hr->setFrameShape(QFrame::HLine);
        hr->setFrameShadow(QFrame::Sunken);
        ui->gridLayout->addWidget(hr, row, 0, 1, 2);
        row++;
    }

    if(!invertebrate.order.isEmpty()) {
        ui->gridLayout->addWidget(new QLabel("Order:"), row, 0);
        ui->gridLayout->addWidget(new QLabel(invertebrate.order), row, 1);
        row++;
        QFrame *hr = new QFrame();
        hr->setFrameShape(QFrame::HLine);
        hr->setFrameShadow(QFrame::Sunken);
        ui->gridLayout->addWidget(hr, row, 0, 1, 2);
        row++;
    }

    if(!invertebrate.flyName.isEmpty()) {
        ui->gridLayout->addWidget(new QLabel("Tied Fly:"), row, 0);
        ui->gridLayout->addWidget(new QLabel(invertebrate.flyName), row, 1);
        row++;
        QFrame *hr = new QFrame();
        hr->setFrameShape(QFrame::HLine);
        hr->setFrameShadow(QFrame::Sunken);
        ui->gridLayout->addWidget(hr, row, 0, 1, 2);
        row++;
    }

    ui->gridLayout->setColumnStretch(1, 1);
    ui->descriptionLabel->setText(invertebrate.description);

//    ui->centralwidget->ensurePolished();

    QPixmap pixmap;

    switch (invertebrate.imageIsReady) {
        case ImageStatus::READY:
            pixmap = QPixmap(invertebrate.imageFileLocal);
            break;
        case ImageStatus::QUEUED_FOR_DOWNLOAD:
            pixmap = QPixmap(":/media/placeholder-queued.png");
            break;
        default:
            pixmap = QPixmap(":/media/placeholder-unavailable.png");
            break;
    }

#ifdef ON_IOS
    pixmap = pixmap.scaledToWidth(qApp->primaryScreen()->geometry().size().width() * 0.85);
#endif
    ui->imageLabel->setPixmap(pixmap);
}

void InvertebrateView::on_pushButton_back_pressed()
{
    emit backButtonClicked(originStream);
}
