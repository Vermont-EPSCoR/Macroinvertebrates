#include "homeview.h"
#include "ui_homeview.h"

#include <QDebug>

HomeView::HomeView(QWidget* parent):
    QWidget(parent),
    ui(new Ui::HomeView)
{
    ui->setupUi(this);
}

#ifdef MOBILE_DEPLOYMENT
void HomeView::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    QSize viewDimensions = qApp->screens().first()->availableSize();

    int taken_space = ui->syncButton->sizeHint().height() + (ui->startButton->sizeHint().height() * 2);
    bool mustResizeLogo = ((viewDimensions.height() - taken_space) <= epscorLogoHeight || viewDimensions.width() <= epscorLogoWidth) ? true : false;

    if( mustResizeLogo ) {
        QPixmap scaled_pixmap;
        QSettings settings;

        bool is_wide = (viewDimensions.width() > viewDimensions.height()) ? true : false;
        if (is_wide) {
            QVariant data = settings.value("epscor_logo_wide");

            if (data.isNull()) {
                scaled_pixmap = QPixmap::fromImage(QImage(":/media/logo.png")).scaled(
                            static_cast<int>(viewDimensions.width() * 0.25),
                            static_cast<int>(viewDimensions.height() * 0.25),
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation);

                QByteArray byteArray;
                QBuffer buffer(&byteArray);
                buffer.open(QBuffer::WriteOnly);
                scaled_pixmap.save(&buffer, "PNG");

                settings.setValue("epscor_logo_wide", byteArray);
            } else {
                scaled_pixmap.loadFromData(data.toByteArray(), "PNG");
            }
        } else {
            QVariant data = settings.value("epscor_logo_tall");

            if(data.isNull()) {
                scaled_pixmap = QPixmap::fromImage(QImage(":/media/logo.png")).scaled(
                            static_cast<int>(viewDimensions.width() * 0.85),
                            static_cast<int>(viewDimensions.height() * 0.85),
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation);

                QByteArray byteArray;
                QBuffer buffer(&byteArray);
                buffer.open(QBuffer::WriteOnly);
                scaled_pixmap.save(&buffer, "PNG");

                settings.setValue("epscor_logo_tall", byteArray);
            } else {
                // Using stored tall version
                scaled_pixmap.loadFromData(data.toByteArray(), "PNG");
            }
        }

        ui->label->setPixmap(scaled_pixmap);
    } else {
        // Using full size image
        ui->label->setPixmap(QPixmap::fromImage(QImage(":/media/logo.png")));
    }
}
#endif

HomeView::~HomeView()
{
    delete ui;
}

void HomeView::on_syncButton_pressed()
{
    emit syncButtonClicked();
}

void HomeView::on_startButton_pressed()
{
    emit startButtonClicked();
}

void HomeView::on_aboutButton_pressed()
{
    emit aboutButtonClicked();
}

void HomeView::on_backButton_pressed()
{
    emit backButtonPressed();
}
