#include "streamview.h"
#include "ui_streamview.h"

StreamView::StreamView(const std::vector<QString> &streamList, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StreamView)
{
    ui->setupUi(this);
    QFont default_font = qApp->font();

    QVariant OvershootPolicy = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
    QScrollerProperties ScrollerProperties = QScroller::scroller(ui->listWidget->viewport())->scrollerProperties();
    ScrollerProperties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, OvershootPolicy);
    ScrollerProperties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, OvershootPolicy);
    QScroller::scroller(ui->listWidget->viewport())->setScrollerProperties(ScrollerProperties);
    QScroller::grabGesture(ui->listWidget->viewport(), QScroller::LeftMouseButtonGesture);

    for(const QString& stream: streamList) {
        QListWidgetItem* item = new QListWidgetItem(stream);
        item->setFont(default_font);  // Android resizes fonts in QListWidgetItems if font isn't explicitly set
        ui->listWidget->addItem(item);
    }

    int list_view_last_index = static_cast<Application *>(qApp)->last_view_index();
    if(list_view_last_index != -1) {
        ui->listWidget->setCurrentRow(list_view_last_index);
    }
}

StreamView::~StreamView()
{
    static_cast<Application *>(qApp)->set_last_view_index(ui->listWidget->currentRow());
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
