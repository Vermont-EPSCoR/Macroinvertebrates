#include "settingsview.h"
#include "ui_settingsview.h"

SettingsView::SettingsView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsView)
{
    ui->setupUi(this);
    QSettings settings;
    int syncingPreference = settings.value("syncingPreference", QVariant(static_cast<int>(SyncOptions::MANUAL_ONLY))).toInt();

    QStringList syncingOptions({
                                   "Manual Only",  // SyncOptions::MANUAL_ONLY
                                   "On Startup (Always)",  // SyncOptions::ON_STARTUP
                                   "On Startup (WiFi)",  // SyncOptions::WIFI
                               });
    ui->comboBox->addItems(syncingOptions);
    // Disable On Startup
    // Horrible hack according to peppe (who is one of the Qt developers)
    ui->comboBox->setItemData(2, QVariant(0), Qt::UserRole - 1);
    ui->comboBox->setCurrentIndex(syncingPreference);
}

SettingsView::~SettingsView()
{
    delete ui;
}

void SettingsView::on_pushButton_2_clicked()
{
    emit backButtonClicked();
}

void SettingsView::on_pushButton_clicked()
{
    emit syncButtonClicked();
}

void SettingsView::on_comboBox_currentIndexChanged(int index)
{
    QSettings settings;
    settings.setValue("syncingPreference", index);
    settings.sync();
}

void SettingsView::updateLastSync()
{
    QSettings settings;
    ui->lastUpdateLabel->setText(settings.value("lastUpdate").toString());
}

void SettingsView::toggleSyncButtonText(SyncStatus desiredState)
{
    if(desiredState == SyncStatus::READY_TO_SYNC) {
        ui->pushButton->setText("Sync Now");
    } else {
        ui->pushButton->setText("Cancel Sync");
    }
}
