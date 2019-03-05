#include "security.h"
#include "QFile"
//#include<QMessageBox>
#include <QTextStream>
#include "securityutils.h"
//#include <QDebug>


Security::Security(){

}
Security::~Security(){

}
bool Security::checkSafety(){

    QString filepath = "/sys/fsl_otp/HW_OCOTP_CFG0";
    QString cfg, mac;
    readRegister(filepath, cfg);
    // ui->cfg0Edit->setText(cfg);
    securityDeal(cfg, mac);
    //ui->value0Edit->setText(mac);
    /*write mac to /sys/fsl_otp/HW_OCOTP_MAC0*/
    //    QString cmd = "echo 0x"+mac+"> /sys/fsl_otp/HW_OCOTP_MAC0";
    //    const char* command = cmd.toUtf8().constData() ;
    //    system(command);

    filepath = "/sys/fsl_otp/HW_OCOTP_MAC0";
    readRegister(filepath, cfg);
    // ui->mac0Edit->setText(cfg);
    bool ok;
    long long  hex = cfg.toLongLong(&ok, 0);       // hex == 255, ok == true
    QString hexadecimal;
    hexadecimal.setNum(hex,16);
    if(hexadecimal != mac)
    {
        // QMessageBox::warning(0, "error", "加密失败，MAC0写错误！");
        return false;
    }else{
        return true;
    }
}

void Security::securityDeal(QString &cfg, QString &mac)
{
    bool ok;
    long long  hex = cfg.toLongLong(&ok, 0);       // hex == 255, ok == true
    unsigned char sha512Code[64];
    QString hexadecimal;
    hexadecimal.setNum(hex,16);
    QByteArray ba = hexadecimal.toLatin1();
    unsigned  char *input =  (unsigned char*)ba.data();
    //qDebug() <<"hexadecimal = " << hexadecimal;
    FIPS202_SHA3_512((const unsigned char *)input,8,sha512Code);
    QByteArray data = QByteArray::fromRawData((char*)sha512Code, sizeof(sha512Code));
    //    QByteArray hash = QCryptographicHash::hash(hexadecimal.toUtf8(), QCryptographicHash::Sha3_512);
    //    QString password(hash.toHex());
    QString password(data.toHex());
    mac = password.mid(8,4)+password.mid(40,4);
}
void Security::readRegister(QString filepath, QString &cfg)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)) {
        // QMessageBox::information(0, "error", file.errorString());
        //qDebug(file.errorString())  ;
    }
    QTextStream in(&file);
    cfg= in.readLine();
    file.close();
}
