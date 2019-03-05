#ifndef WIFIPAGE_H
#define WIFIPAGE_H

#include <QDialog>
#include "app/wifiinfo.h"
#include "wifiitem.h"
#include <QListWidgetItem>
#include <QLabel>
#include <QSettings>
#include "keypad.h"

namespace Ui {
class WifiPage;
}

class WifiPage : public QDialog
{
    Q_OBJECT

public:
    explicit WifiPage(QWidget *parent = 0);
    ~WifiPage();

private slots:
    void onReadWifiOver(QStringList wifis,QStringList psws);
    void showDelDialog(int index);
    void onWifiConnected();
    void on_back_clicked();

    bool showKeyPad(QLineEdit *line);
    bool eventFilter(QObject *, QEvent *);
    void on_addWifi_clicked();

    void on_listWidget_clicked(const QModelIndex &index);

    void on_listWidget_pressed(const QModelIndex &index);

    void on_cancelDele_clicked();

    void on_deleteWifi_clicked();

    void on_cancelButton_clicked();

    void on_commitButton_clicked();

    void on_nameEdit_selectionChanged();

private:
    Ui::WifiPage *ui;
   // bool showDialog; //是否显示添加wifi的对话框
    WifiInfo *wifiInfo;
    int lastWifi; //上一次点击的wifi位置
    QSettings settings;
    WifiItem *widget;
    QListWidgetItem *nitem;
    QList<QLabel*> Labels;
    QStringList wifis;
    QStringList psws;
    int delIndex;
    QString connectedName; //已连接的wifi名称
    QString connectedIp;  //已连接的wifiIp
    void resetItemColor(int lastIndex,int currentIndex);
    Keypad *keybod;

};

#endif // WIFIPAGE_H
