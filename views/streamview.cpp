#include "streamview.h"
#include "ui_streamview.h"

StreamView::StreamView(const std::vector<QString> &streamList, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StreamView)
{
    ui->setupUi(this);

    if( qApp->desktop()->size().width() < 500) {
        qDebug() << "Doing this";
        QVariant OvershootPolicy = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
        QScrollerProperties ScrollerProperties = QScroller::scroller(ui->listWidget->viewport())->scrollerProperties();
        ScrollerProperties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, OvershootPolicy);
        ScrollerProperties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, OvershootPolicy);
        QScroller::scroller(ui->listWidget->viewport())->setScrollerProperties(ScrollerProperties);
        QScroller::grabGesture(ui->listWidget->viewport(), QScroller::LeftMouseButtonGesture);
    }

    for(const QString& stream: streamList) {
        ui->listWidget->addItem(stream);
    }
}

StreamView::~StreamView()
{
    delete ui;
}

void StreamView::on_backButton_pressed()
{
    emit backButtonClicked();
}

void StreamView::on_listWidget_itemClicked(QListWidgetItem *item)
{
    emit singleStreamClicked(item->text());
}

void StreamView::on_streamSearch_textEdited(const QString &arg1)
{
    int rowCount = ui->listWidget->model()->rowCount();
    for(int i = 0; i < rowCount; i++) {
        QListWidgetItem *item = ui->listWidget->item(i);

        if(arg1.isEmpty()) {
            item->setHidden(false);
        } else if(item->text().contains(arg1, Qt::CaseInsensitive)) {
            item->setHidden(false);
        } else {
            item->setHidden(true);
        }
    }
}
