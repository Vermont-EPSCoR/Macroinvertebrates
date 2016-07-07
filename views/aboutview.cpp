#include "aboutview.h"
#include "ui_aboutview.h"

AboutView::AboutView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutView)
{
    ui->setupUi(this);
    QSettings settings;
    QString about = settings.value("aboutText").toString();

    if(!about.isEmpty()) {
        ui->label->setText(about);
    }

    QVariant OvershootPolicy = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
    QScrollerProperties ScrollerProperties = QScroller::scroller(ui->scrollArea->viewport())->scrollerProperties();
    ScrollerProperties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, OvershootPolicy);
    ScrollerProperties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, OvershootPolicy);
    QScroller::scroller(ui->scrollArea->viewport())->setScrollerProperties(ScrollerProperties);
    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
}

AboutView::~AboutView()
{
    delete ui;
}

void AboutView::updateAbout(const QString &about)
{
    ui->label->setText(about);
}

void AboutView::on_backButton_pressed()
{
    emit backButtonClicked();
}
