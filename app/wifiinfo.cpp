#include "wifiinfo.h"
#include "qfile.h"
#include "qtextstream.h"
#include "QDebug"

WifiInfo::WifiInfo(QWidget *parent)
{
    m_workTimer = new QTimer;
    connect(m_workTimer, SIGNAL(timeout()), this, SLOT(refreshWifiStatus()));
    //textName="/home/jia/公共的/wifiList.txt";
    //textPath="/home/jia/公共的/";
    textName="/home/root/music/output/wifiList.txt";
    textPath="/home/root/music/output/";

    splitTag="!~@PSW@~!";
    lastIp="";
    hasRunNetPage=false;
    flag=0;
}
WifiInfo::~WifiInfo()
{
    delete m_workTimer;
}
void WifiInfo::addWifi(QString name,QString password){
    qDebug()<<"---addWifi---";
    QFile f(textName);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qDebug()<<"Open failed."<< endl;
        return ;
    }
    QTextStream txtOutput(&f);
    txtOutput<<name<<splitTag<<password<<endl;
    f.close();
    system("sync");
    // readWifiList();
}
void WifiInfo::delWifi(QString name,QString password){
    QFile f(textName);
    QStringList wifis;
    QString del=name+splitTag+password;
    //  qDebug()<<"~~~~~delWifi~~~~~~~~~~";
    if(!f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        //  qDebug()<<"Open failed."<< endl;
    }
    QTextStream txtInput(&f);
    QString lineStr;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        wifis.append(lineStr);
    }
    wifis.removeOne(del);
    f.resize(0);
    for(int i=0;i<wifis.length();i++){
        // qDebug()<<"~~~~~~~~~~~~~~~"<<wifis.at(i)<< endl;
        txtInput<<wifis.at(i)<<endl;
    }
    f.close();
    system("sync");
    //qDebug()<<"~~~~~~~~~~~~~~~"<<strall<< endl;

}
void WifiInfo::readWifiList(){
    qDebug()<<"---------readingWifiList";
    QFile f(textName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
         qDebug()<<"Open failed."<< endl;
    }
    QTextStream txtInput(&f);
    QString lineStr;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        wifiList.append(lineStr.split(splitTag)[0]);
        //qDebug()<<lineStr;
        pswList.append(lineStr.split(splitTag)[1]);
    }
    f.close();
    system("sync");
    emit readWifiOver(wifiList,pswList);
    //qDebug()<<"readWifiOver";
}
void WifiInfo::connetWifi(QString name,QString password){
    hasSendConnected=false;
    hasSendDisConnected=false;
    hasRunNetPage=false;
    m_wifiInfo[ESSID_IP]="";
    // setRefresh(false);
    setCurrentWifi(name,password);
    system("killall wpa_supplicant");
    system("killall udhcpc");
    system("ifconfig wlan0 up");
    system("wpa_supplicant -iwlan0 -B -c /etc/wpa_supplicant.conf &");
    system("udhcpc -i wlan0 &");
    // system("killall noVnc");
    // setRefresh(true);
}
void WifiInfo::checkWifiStatus(){
    hasSendDisConnected=false;
    hasSendConnected=false;
    setRefresh(true);
}


QString WifiInfo::getCurrentWifiIp(){
    return m_wifiInfo[ESSID_IP];
}
QString WifiInfo::getCurrentWifiName(){
    return m_wifiInfo[ESSID_SSID];
}



void WifiInfo::setCurrentWifi(const QString ssid, const QString password)
{
    //wpa_passphrase "RoadJ" "tozr7000" >> /etc/wpa_supplicant.conf
    char cmdbuf[MAX_PATH];
    char cmdresult[MAX_PATH];
    FILE* fp=fopen("/etc/wpa_supplicant.conf", "wb");
    fprintf(fp, "ctrl_interface=/var/run/wpa_supplicant\nctrl_interface_group=0\nap_scan=1\n\n");
    sprintf(cmdbuf, "wpa_passphrase %s %s", ssid.toLatin1().data(), password.toLatin1().data());
    FILE *pp = popen(cmdbuf, "r"); //建立管道
    while(fgets(cmdresult, sizeof(cmdresult), pp))  //""
    {
        fputs(cmdresult, fp);
    }
    pclose(pp);
    fclose(fp);
}
void WifiInfo::getWifiInfo(QString &ssid, QString &ip)
{
    ssid = m_wifiInfo[ESSID_SSID];
    ip = m_wifiInfo[ESSID_IP];
}
void WifiInfo::getWifiStatus(QString &status)
{
    status = m_wifiInfo[ESSID_STATUS];
}
void WifiInfo::refreshWifiStatus()
{
    //从 status 中读取
    char result[MAX_LEN];
    char key[MAX_LEN]; //设置一个合适的长度，以存储每一行输出
    char value[MAX_LEN]; //设置一个合适的长度，以存储每一行输出
    bzero(result, MAX_LEN);
    bzero(key, MAX_LEN);
    bzero(value, MAX_LEN);
    FILE *pp = popen("wpa_cli -iwlan0 status", "r"); //建立管道
    if (!pp)
        return;
    while( fgets(result, sizeof(result), pp) != NULL)
    {
        sscanf(result, "%[^=]=%s", key, value);
        //如果这里不用QString包含，会对比地址
        if(QString("wpa_state") == QString(key)) {
            m_wifiInfo[ESSID_STATUS] = value;
            //qDebug()<<"----------"<<value;
        }
        else if(QString("bssid") == QString(key))
        {
            m_wifiInfo[ESSID_BSSID] = value;
        }
        else if(QString("ssid") == QString(key))
        {
            m_wifiInfo[ESSID_SSID] = value;
            // qDebug()<<"----------"<<value;
        }
        else if(QString("ip_address") == QString(key))
        {
            m_wifiInfo[ESSID_IP] = value;
            // qDebug()<<"----------"<<value;
        }
    }
    if(m_wifiInfo[ESSID_STATUS] =="SCANNING"){
        m_wifiInfo[ESSID_IP]="";
        //qDebug()<<"----SCANNING------"<<m_wifiInfo[ESSID_SSID];
        // emit wifiConnecting(m_wifiInfo[ESSID_SSID]);

    }
    if(m_wifiInfo[ESSID_STATUS] == "COMPLETED")
    {
        QString ip = m_wifiInfo[ESSID_IP];
        if(!hasSendConnected&&!ip.isEmpty()&&ip.length()>0){
            if(!hasRunNetPage){
                //与上一次ip相同不重新设置网页版
                //                if(lastIp==ip){
                //                    hasRunNetPage=true;
                //                    return;
                //                }
                //                // qDebug()<<"----set net page-----"<<endl;
                //                char  *chUmount;
                //                QString orderUmount="./noVNC/utils/launchport4080.sh --vnc "+m_wifiInfo[ESSID_IP]+":5900 &";
                //                QByteArray ba=orderUmount.toLatin1();
                //                chUmount=ba.data();
                //                system("cd /usr/src/");
                //                system(chUmount);
                //                hasRunNetPage=true;
                //                lastIp=ip;
            }
            emit wifiConnected();
            hasSendConnected=true;
            hasSendDisConnected=false;
        }
        //        if(flag==2&&!hasSendConnected){
        //            qDebug()<<"--COMPLETED-ip is-----"<< m_wifiInfo[ESSID_IP];
        //            emit wifiConnected();
        //            hasSendConnected=true;
        //            flag=0;
        //        }
        //        if(!hasSendConnected){
        //            qDebug()<<"--!!COMPLETED-ip is-----"<< m_wifiInfo[ESSID_IP];
        //            flag++;
        //        }

        //        QString ip = m_wifiInfo[ESSID_IP];
        //        if(!ip.isEmpty()&&ip.length()>0&&!hasSendConnected&&ip!=lastIp){
        //            emit wifiConnected();
        //            if(!hasRunNetPage){
        //                //检查的时候不应该重新设置网页版
        //                qDebug()<<"----set net page-----"<<endl;
        //                char  *chUmount;
        //                QString orderUmount="./noVNC/utils/launchport4080.sh --vnc "+m_wifiInfo[ESSID_IP]+" &";
        //                QByteArray ba=orderUmount.toLatin1();
        //                chUmount=ba.data();
        //                system("cd /usr/src/");
        //                system(chUmount);
        //                hasRunNetPage=true;
        //            }
        //

        // system("./noVNC/utils/launchport4080.sh --vnc "+ip);
        //            qDebug()<<"--COMPLETED-ip is-----"<< m_wifiInfo[ESSID_IP];
        //            qDebug()<<"--COMPLETED-ip name-----"<< m_wifiInfo[ESSID_SSID];
        //            hasSendConnected=true;
        //            hasSendDisConnected=false;
        //            lastIp=ip;
        //     }

    }else {
        if(!hasSendDisConnected){
            emit wifiDisConnected();
            hasSendDisConnected=true;
            hasSendConnected=false;

        }

        //setRefresh(false);
    }
    pclose(pp);
}
