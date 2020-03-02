#include "invertebrateview.h"
#include "ui_invertebrateview.h"

InvertebrateView::InvertebrateView(const Invertebrate &invertebrate, const QString &streamName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InvertebrateView)
{
    ui->setupUi(this);
    ui->scrollArea->setWidgetResizable(true);
    QVariant OvershootPolicy = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
    QScrollerProperties properties = QScroller::scroller(ui->scrollArea->viewport())->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, OvershootPolicy);
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, OvershootPolicy);

    properties.setScrollMetric(QScrollerProperties::MaximumVelocity, QVariant(0.0001));

    QScroller::scroller(ui->scrollArea->viewport())->setScrollerProperties(properties);
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

//    bool viewIsNarrow = (qApp->desktop()->availableGeometry().size().width() < 400);
    bool viewIsNarrow = (QGuiApplication::primaryScreen()->availableGeometry().size().width() < 400);

    int row = 0;
    if(!invertebrate.commonName.isEmpty()) {
        addInfoToLayout(viewIsNarrow, "Common Name:", invertebrate.commonName, row);
    }

    if(!invertebrate.family.isEmpty()) {
        addInfoToLayout(viewIsNarrow, "Family:", invertebrate.family, row);
    }

    if(!invertebrate.genus.isEmpty()) {
        addInfoToLayout(viewIsNarrow, "Genus:", invertebrate.genus, row);
    }

    if(!invertebrate.order.isEmpty()) {
        addInfoToLayout(viewIsNarrow, "Order:", invertebrate.order, row);
    }

    if(!invertebrate.flyName.isEmpty()) {
        addInfoToLayout(viewIsNarrow, "Tied Fly:", invertebrate.flyName, row);
    }

    ui->gridLayout->setColumnStretch(1, 1);
    ui->descriptionLabel->setText(invertebrate.description);

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

//#ifdef IOS_SPECIFC
    pixmap = pixmap.scaledToWidth(qApp->primaryScreen()->geometry().size().width() * 0.85);
//#endif
    image = pixmap;
    ui->imageLabel->setPixmap(pixmap);
}

void InvertebrateView::on_pushButton_back_pressed()
{
    emit backButtonClicked(originStream);
}

void InvertebrateView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ui->imageLabel->setPixmap(image.scaled(qApp->primaryScreen()->geometry().size() * 0.85, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void InvertebrateView::addInfoToLayout(bool isNarrow, const QString &label, const QString &value, int &active_row)
{
    isNarrow = true;
    int valueCol = (isNarrow) ? 0 : 1;
    ui->gridLayout->addWidget(new QLabel(label), active_row, 0);

    if(isNarrow) {
        active_row++;
    }

    ui->gridLayout->addWidget(new QLabel(value), active_row, valueCol);
    active_row++;

    // Add a horizontal rule
    QFrame *hr = new QFrame();
    hr->setFrameShape(QFrame::HLine);
    hr->setFrameShadow(QFrame::Sunken);
    ui->gridLayout->addWidget(hr, active_row, 0, 1, 2);
    active_row++;
}
