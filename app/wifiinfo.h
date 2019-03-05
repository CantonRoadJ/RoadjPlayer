#ifndef WIFIINFO_H
#define WIFIINFO_H

#include<QWidget>
#include<QString>
#include<QTimer>
#include<QMap>
#include <stdio.h>

#define MAX_LEN              255
#define MAX_PATH            1024
#define ESSID_BSSID         "bssid"
#define ESSID_PASS           "password"
#define ESSID_SSID           "ssid"
#define ESSID_IP                "ip"
#define ESSID_STATUS       "status"
#define ESSID_TYPE           "type"
#define ESSID_ENCRYP      "encryp"
#define ESSID_FREQ          "frequency"
#define ESSID_SIGNAL      "signal"
#define ESSID_FLAG          "flag"

#define MWIFI   QMap<QString, QString>

class WifiInfo : public QObject
{
    Q_OBJECT
public:
    explicit WifiInfo(QWidget *parent = 0);
    ~WifiInfo();

    Q_INVOKABLE void addWifi(QString name,QString password);
    Q_INVOKABLE void delWifi(QString name,QString password);
    Q_INVOKABLE void connetWifi(QString name,QString password);
    Q_INVOKABLE void checkWifiStatus();
    Q_INVOKABLE void readWifiList();
    //Q_INVOKABLE QStringList readPswList();
    Q_INVOKABLE QString getCurrentWifiIp();
    Q_INVOKABLE QString getCurrentWifiName();


    void setCurrentWifi(const QString ssid, const QString password);
    void setRefresh(bool ref = true) { ref ? m_workTimer->start(5000) : m_workTimer->stop(); }
    void getWifiInfo(QString& ssid, QString& ip);
    void getWifiStatus(QString& status);

    //signals:
Q_SIGNALS:
    void wifiConnected();
    void wifiDisConnected();
    void wifiConnecting(QString connectingName);
    void readWifiOver(QStringList wifis,QStringList psws);

public slots:
    void refreshWifiStatus();
private:
    QTimer *m_workTimer;
    MWIFI m_wifiInfo;
    QList<MWIFI> m_wifiList;
   // bool hasSendResult;
    bool hasSendConnected;
    bool hasSendDisConnected;
    bool hasRunNetPage; //是否设置网页版

    QString textPath;
    QString splitTag;
    QString textName;
    QStringList wifiList;
    QStringList pswList;
    QString lastIp;
    int flag;
};

#endif // WIFIINFO_H
