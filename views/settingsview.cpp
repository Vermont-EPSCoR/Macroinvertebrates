#include "settingsview.h"
#include "ui_settingsview.h"

SettingsView::SettingsView(QWidget *parent) :
    QWidget(parent),
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
    ui->lastSyncLabel->setText(settings.value("lastUpdate").toString());
    ui->syncModeComboBox->addItems(syncingOptions);
    // Disable On Startup
    // Horrible hack according to peppe (who is one of the Qt developers)
    ui->syncModeComboBox->setItemData(2, QVariant(0), Qt::UserRole - 1);
    ui->syncModeComboBox->setCurrentIndex(syncingPreference);
}

SettingsView::~SettingsView()
{
    delete ui;
}

void SettingsView::on_backButton_pressed()
{
    emit backButtonClicked();
}

void SettingsView::on_syncModeComboBox_currentIndexChanged(int index)
{
    QSettings settings;
    settings.setValue("syncingPreference", index);
    settings.sync();
}

void SettingsView::on_syncButton_pressed()
{
    emit syncButtonClicked();
}

void SettingsView::toggleSyncButtonText(SyncStatus desiredState)
{
    if(desiredState == SyncStatus::READY_TO_SYNC) {
        ui->syncButton->setText("Sync Now");
    } else {
        ui->syncButton->setText("Cancel Sync");
    }
}
