#include "singlestreamview.h"
#include "ui_singlestreamview.h"

SingleStreamView::SingleStreamView(const std::vector<Invertebrate> &invertebratesList, const QString &streamName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleStreamView)
{
    ui->setupUi(this);
    ui->streamNameLabel->setText(streamName);
    this->streamName = streamName;

    const double the_golden_ratio  = 1.618;
    double desired_ratio = 1.0;
    QSettings settings;
    QSize screen_size = settings.value("screen_size").toSize();
    QSize icon_size;

    desired_ratio = 0.3 * the_golden_ratio;
    icon_size = screen_size * desired_ratio;
    icon_size.setWidth(icon_size.width() / the_golden_ratio );
    ui->listWidget->setIconSize(icon_size);

    // Allow inertial scrolling
    QVariant OvershootPolicy = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
    QScrollerProperties ScrollerProperties = QScroller::scroller(ui->listWidget->viewport())->scrollerProperties();
    ScrollerProperties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, OvershootPolicy);
    ScrollerProperties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, OvershootPolicy);
    QScroller::scroller(ui->listWidget->viewport())->setScrollerProperties(ScrollerProperties);
    QScroller::grabGesture(ui->listWidget->viewport(), QScroller::LeftMouseButtonGesture);

    for(const Invertebrate& invertebrate: invertebratesList) {
        QListWidgetItem *item = nullptr;

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

    QFile css_file(":/styles/portrait_view.css");
    if(css_file.open(QFile::ReadOnly | QFile::Text)) {
        portrait_css = css_file.readAll();
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

void SingleStreamView::resizeEvent(QResizeEvent *event)
{
#ifdef IOS_SPECIFIC
    Q_UNUSED(event);
    Qt::ScreenOrientation orientation = qApp->screens().first()->orientation();
    if(orientation == Qt::PortraitOrientation || orientation == Qt::InvertedPortraitOrientation) {
        setStyleSheet(portrait_css);
    } else {
        setStyleSheet("/**/");
    }
#endif
}
