#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QMainWindow>
#include <QSettings>
#include <QDebug>
#include <QDateTime>

#include "../webdatasynchronizer.h"

namespace Ui {
class SettingsView;
}

class SettingsView : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsView(QWidget *parent = 0);
    ~SettingsView();
    void updateLastSync();
    void toggleSyncButtonText(SyncStatus desiredState);
signals:
    void backButtonClicked();
    void syncButtonClicked();
    void reloadStylesClicked();
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::SettingsView *ui;
};

#endif // SETTINGSVIEW_H
