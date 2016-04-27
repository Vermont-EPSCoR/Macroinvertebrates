#include "settingsview.h"
#include "ui_settings.h"

SettingsView::SettingsView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsView)
{
    ui->setupUi(this);
    QSettings settings;
    int syncingPreference = settings.value("syncingPreference", QVariant(static_cast<int>(SyncOptions::MANUAL_ONLY))).toInt();

    QStringList syncingOptions({
                                   "Manual Only",  // SyncOptions::MANUAL_ONLY
                                   "Always On Startup",  // SyncOptions::ON_STARTUP
                                   "Always On Startup (WiFi)",  // SyncOptions::WIFI
                               });
    ui->comboBox->addItems(syncingOptions);
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
    settings.setValue("syncingPreference", static_cast<int>(index));
}

void SettingsView::updateLastSync()
{
    QSettings settings;
    ui->label->setText(settings.value("lastUpdate").toString());
}
