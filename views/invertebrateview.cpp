#include "invertebrateview.h"
#include "ui_invertebrateview.h"

/**
 * Helper function. Removes all layout items within the given @a layout
 * which either span the given @a row or @a column. If @a deleteWidgets
 * is true, all concerned child widgets become not only removed from the
 * layout, but also deleted.
 */
void remove(QGridLayout *layout, int row, int column, bool deleteWidgets) {
    // We avoid usage of QGridLayout::itemAtPosition() here to improve performance.
    for (int i = layout->count() - 1; i >= 0; i--) {
        int r, c, rs, cs;
        layout->getItemPosition(i, &r, &c, &rs, &cs);
        if ((r <= row && r + rs - 1 >= row) || (c <= column && c + cs - 1 >= column)) {
            // This layout item is subject to deletion.
            QLayoutItem *item = layout->takeAt(i);
            if (deleteWidgets) {
                deleteChildWidgets(item);
            }
            delete item;
        }
    }
}

/**
 * Helper function. Deletes all child widgets of the given layout @a item.
 */
void deleteChildWidgets(QLayoutItem *item) {
    if (item->layout()) {
        // Process all child items recursively.
        for (int i = 0; i < item->layout()->count(); i++) {
            deleteChildWidgets(item->layout()->itemAt(i));
        }
    }
    delete item->widget();
}

/**
 * Removes all layout items on the given @a row from the given grid
 * @a layout. If @a deleteWidgets is true, all concerned child widgets
 * become not only removed from the layout, but also deleted. Note that
 * this function doesn't actually remove the row itself from the grid
 * layout, as this isn't possible (i.e. the rowCount() and row indices
 * will stay the same after this function has been called).
 */
void removeRow(QGridLayout *layout, int row, bool deleteWidgets) {
    remove(layout, row, -1, deleteWidgets);
    layout->setRowMinimumHeight(row, 0);
    layout->setRowStretch(row, 0);
}

/**
 * Removes all layout items on the given @a column from the given grid
 * @a layout. If @a deleteWidgets is true, all concerned child widgets
 * become not only removed from the layout, but also deleted. Note that
 * this function doesn't actually remove the column itself from the grid
 * layout, as this isn't possible (i.e. the columnCount() and column
 * indices will stay the same after this function has been called).
 */
void removeColumn(QGridLayout *layout, int column, bool deleteWidgets) {
    remove(layout, -1, column, deleteWidgets);
    layout->setColumnMinimumWidth(column, 0);
    layout->setColumnStretch(column, 0);
}

InvertebrateView::InvertebrateView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InvertebrateView)
{
    ui->setupUi(this);
    ui->description->setReadOnly(true);
}

InvertebrateView::~InvertebrateView()
{
    delete ui;
}

void InvertebrateView::setInfo(const Invertebrate &invertebrate, QString streamName)
{
#ifndef MOBILE_DEPLOYMENT
    qDebug() << invertebrate;
#endif
    int rows = ui->gridLayout->rowCount();
    for(int i = 0; i < rows; i++) {
        removeRow(ui->gridLayout, i, true);
    }

    this->streamName = streamName;
    ui->label_title->setText(invertebrate.name);

    int row = 0;
    if(!invertebrate.commonName.isEmpty()) {
        ui->gridLayout->addWidget(new QLabel("Common Name:"), row, 0);
        ui->gridLayout->addWidget(new QLabel(invertebrate.commonName), row, 1);
//        ui->gridLayout->addItem(new QSpacerItem());
        row++;
    }

    if(!invertebrate.family.isEmpty()) {
        ui->gridLayout->addWidget(new QLabel("Family:"), row, 0);
        ui->gridLayout->addWidget(new QLabel(invertebrate.family), row, 1);
        row++;
    }

    if(!invertebrate.genus.isEmpty()) {
        ui->gridLayout->addWidget(new QLabel("Genus:"), row, 0);
        ui->gridLayout->addWidget(new QLabel(invertebrate.genus), row, 1);
        row++;
    }

    if(!invertebrate.order.isEmpty()) {
        ui->gridLayout->addWidget(new QLabel("Order:"), row, 0);
        ui->gridLayout->addWidget(new QLabel(invertebrate.order), row, 1);
        row++;
    }

    if(!invertebrate.flyName.isEmpty()) {
        ui->gridLayout->addWidget(new QLabel("Tied Fly:"), row, 0);
        ui->gridLayout->addWidget(new QLabel(invertebrate.flyName), row, 1);
        row++;
    }

    ui->gridLayout->setColumnStretch(1, 1);

    ui->description->setPlainText(invertebrate.description);

    if(invertebrate.imageIsReady) {
        ui->label_image->setPixmap(QPixmap(invertebrate.imageFileLocal));
    } else {
        ui->label_image->setPixmap(QPixmap(":/media/invertebrate-placeholder.jpg"));
    }
}

void InvertebrateView::on_pushButton_back_clicked()
{
    emit backButtonClicked(streamName);
}
