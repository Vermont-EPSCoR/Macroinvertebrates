#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QWidget>
#include <QSettings>
#include <QDebug>
#include <QDateTime>

#include "../webdatasynchronizer.h"

namespace Ui {
class SettingsView;
}

class SettingsView : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsView(bool isSyncingNow, QWidget *parent = 0);
    ~SettingsView();
    void toggleSyncButtonText(SyncStatus desiredState);
signals:
    void backButtonClicked();
    void syncButtonClicked();
private slots:
    void on_backButton_pressed();
    void on_syncModeComboBox_currentIndexChanged(int index);
    void on_syncButton_pressed();

private:
    Ui::SettingsView *ui;
};

#endif // SETTINGSVIEW_H
