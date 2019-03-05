#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include "app/music.h"
#include "eqsettingspage.h"
namespace Ui {
class Dialog;
}

class SettingsPage : public QDialog
{
    Q_OBJECT

public:
    SettingsPage(QWidget *parent = 0,Music *music=0);
    //SettingsPage(Music *music);
    ~SettingsPage();

private slots:
    void on_back_clicked();

    void on_wifiSettings_clicked();

    void on_eqSettings_pressed();

    void on_eqSettings_released();

    void pressTimeOut();



signals:
    void openEq();
    void closeEq();
private:
    Ui::Dialog *ui;
    QTimer *pressTimer;
    int holdTime;

    Music *music;
    QSettings settings;
    bool isEqOpen;
};

#endif // DIALOG_H
