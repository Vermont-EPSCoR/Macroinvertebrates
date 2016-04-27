#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QMainWindow>
#include <QSettings>
#include <QDebug>
#include <QDateTime>

namespace Ui {
class SettingsView;
}

enum class SyncOptions {
    MANUAL_ONLY = 0,
    ON_STARTUP,
    WIFI
};

class SettingsView : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsView(QWidget *parent = 0);
    ~SettingsView();
    void updateLastSync();
signals:
    void backButtonClicked();
    void syncButtonClicked();
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::SettingsView *ui;
};

#endif // SETTINGSVIEW_H
