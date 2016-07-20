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
    QSize desktopSize = qApp->desktop()->size();

    if( !(desktopSize.width() > 500 && desktopSize.height() > 500) ) {
        QRect rect = geometry();
        QPixmap scaled_pixmap;
        QSettings settings;

        bool is_wide = (rect.width() > rect.height()) ? true : false;
        if (is_wide) {
            QVariant data = settings.value("epscor_logo_wide");

            if (data.isNull()) {
//                qDebug() << "Generating the wide version";
                scaled_pixmap = QPixmap::fromImage(QImage(":/media/logo.png")).scaled(rect.width() * 0.25, rect.height() * 0.25, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                QByteArray byteArray;
                QBuffer buffer(&byteArray);
                buffer.open(QBuffer::WriteOnly);
                scaled_pixmap.save(&buffer, "PNG");

                settings.setValue("epscor_logo_wide", byteArray);
            } else {
//                qDebug() << "Using stored wide version";
                scaled_pixmap.loadFromData(data.toByteArray(), "PNG");
            }
        } else {
            QVariant data = settings.value("epscor_logo_tall");

            if(data.isNull()) {
//                qDebug() << "Generating tall version";
                scaled_pixmap = QPixmap::fromImage(QImage(":/media/logo.png")).scaled(rect.width() * 0.85, rect.height() * 0.85, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                QByteArray byteArray;
                QBuffer buffer(&byteArray);
                buffer.open(QBuffer::WriteOnly);
                scaled_pixmap.save(&buffer, "PNG");

                settings.setValue("epscor_logo_tall", byteArray);
            } else {
//                qDebug() << "Using stored tall version";
                scaled_pixmap.loadFromData(data.toByteArray(), "PNG");
            }
        }

        ui->label->setPixmap(scaled_pixmap);
    } else {
//        qDebug() << "Full size image";
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
