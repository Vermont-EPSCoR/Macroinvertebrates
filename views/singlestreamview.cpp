#include "singlestreamview.h"
#include "ui_singlestreamview.h"

SingleStreamView::SingleStreamView(const std::vector<Invertebrate> &invertebratesList, const QString &streamName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleStreamView)
{
    ui->setupUi(this);
    // Set up for elided strings (ellipsis usage)
    QFont default_font = qApp->font();
    QFontMetrics metrics(default_font);
    QSettings settings;
    QSize screen_size = settings.value("screen_size").toSize();
    int grid_x_available = std::min(screen_size.width(), screen_size.height());
    int grid_y_available = std::max(screen_size.width(), screen_size.height());

// In iOS setting a grid size creates a nice grid.
#ifdef IOS_SPECIFIC
    double number_rows = (grid_y_available < 650) ? 3.0 : 3.3;
    double number_columns = (grid_x_available < 650) ? 1.8: 3.0;

//    qDebug() << "NumRows: " << number_rows;
//    qDebug() << "NumCols: " << number_columns;

    int grid_x = static_cast<int>(
        (grid_x_available * 0.8) / number_columns
    );
    int grid_y = static_cast<int>(
        (grid_y_available * 0.8) / number_rows
    );

//    qDebug() << "grid_x: " << grid_x;
//    qDebug() << "grid_y: " << grid_y;

// In Android setting the grid size makes this unreadably small.
//#elif ANDROID_SPECIFIC
#else
    double number_rows = (grid_y_available < 1300) ? 2.9 : 3.3;
    double number_columns = (grid_x_available < 1300) ? 2.1: 3.0;

//    qDebug() << "NumRows: " << number_rows;
//    qDebug() << "NumCols: " << number_columns;

    int grid_x = static_cast<int>(
        (grid_x_available * 0.95) / number_columns
    );
    int grid_y = static_cast<int>(
        (grid_y_available * 0.8) / number_rows
    );

//    qDebug() << "grid_x: " << grid_x;
//    qDebug() << "grid_y: " << grid_y;
#endif

    ui->listWidget->setGridSize(QSize(grid_x, grid_y));
    ui->streamNameLabel->setText(streamName);
    this->streamName = streamName;

    const double the_golden_ratio  = 1.618;
    double desired_ratio = 1.0;
    desired_ratio = 0.3 * the_golden_ratio;

    QSize icon_size;
    icon_size = screen_size * desired_ratio;
    icon_size.setWidth(static_cast<int>(icon_size.width() / the_golden_ratio ));
    ui->listWidget->setIconSize(icon_size);

    // Allow inertial scrolling
    QVariant OvershootPolicy = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
    QScrollerProperties ScrollerProperties = QScroller::scroller(ui->listWidget->viewport())->scrollerProperties();
    ScrollerProperties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, OvershootPolicy);
    ScrollerProperties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, OvershootPolicy);
    QScroller::scroller(ui->listWidget->viewport())->setScrollerProperties(ScrollerProperties);
    QScroller::grabGesture(ui->listWidget->viewport(), QScroller::LeftMouseButtonGesture);

    // Add invertebrates to list
    int invert_name_width = grid_x - 15;
    for(const Invertebrate& invertebrate: invertebratesList) {
        QListWidgetItem *item = nullptr;

        QString invert_name = metrics.elidedText(invertebrate.name, Qt::ElideRight, invert_name_width);
        if(invertebrate.imageIsReady == ImageStatus::READY) {
            QPixmap local_image(invertebrate.imageFileLocal);
            QSize local_image_size = local_image.size();

            // Note: ideally this calculation could be carried out in the data synchronizer to prevent having to resize at run time.
            if(local_image_size.height() > grid_y || local_image_size.width() > grid_x) {
                int new_max_side = static_cast<int>(std::min(grid_x, grid_y) * 0.8);
                local_image = local_image.scaled(new_max_side, new_max_side, Qt::KeepAspectRatio, Qt::FastTransformation);
            }

            item = new QListWidgetItem(QIcon(local_image), invert_name);
        } else if(invertebrate.imageIsReady == ImageStatus::QUEUED_FOR_DOWNLOAD) {
            item = new QListWidgetItem(QIcon(":/media/placeholder-queued.png"), invert_name);
        } else {
            item = new QListWidgetItem(QIcon(":/media/placeholder-unavailable.png"), invert_name);
        }

        item->setTextAlignment(Qt::AlignCenter);
        item->setSizeHint(QSize(grid_x - 5, grid_y - 5));
        item->setFont(default_font);  // Android resizes fonts in QListWidgetItems if font isn't explicitly set
        ui->listWidget->addItem(item);
    }

    int list_view_last_index = static_cast<Application *>(qApp)->last_view_index();
    if(list_view_last_index != -1) {
        ui->listWidget->setCurrentRow(list_view_last_index);
    }
}

SingleStreamView::~SingleStreamView()
{
    static_cast<Application *>(qApp)->set_last_view_index(ui->listWidget->currentRow());
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
    Q_UNUSED(event);
#ifdef IOS_SPECIFIC
    Qt::ScreenOrientation orientation = qApp->screens().first()->orientation();
    if(orientation == Qt::PortraitOrientation || orientation == Qt::InvertedPortraitOrientation) {
        setStyleSheet(portrait_css);
    } else {
        setStyleSheet("/**/");
    }
#endif
}
