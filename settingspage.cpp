#include "settingspage.h"
#include "ui_settingspage.h"
#include <QDebug>
#include "wifipage.h"

SettingsPage::SettingsPage(QWidget *parent,Music *music) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(QRect(0, 0, 540, 960));//可设置窗口显示的方位与大小
    this->music=music;
    //music->playNext();


    ui->eqSettings->setStyleSheet(
                "QPushButton{color: white;"
                "text-align:left;}"
                "QPushButton:pressed{"
                "background-color:""#2a2c31"";"
                "border-style: flat; }"
                );
    ui->wifiSettings->setStyleSheet(
                "QPushButton{color: white;"
                "text-align:left;}"
                "QPushButton:pressed{"
                "background-color:""#2a2c31"";"
                "border-style: flat; }"
                );
    ui->version->setStyleSheet(
                "QPushButton{color: white;"
                "text-align:left;}"
                "QPushButton:pressed{"
                "background-color:""#2a2c31"";"
                "border-style: flat; }"
                );
    ui->back->setStyleSheet(
                "QPushButton:pressed{"
                "background-color:rgb(37, 38, 43);"
                "border-style: flat; }"
                );
    ui->checkPoint->setStyleSheet("background-color: rgb(206, 92, 0);"
                                  "border-radius: 8px;"
                                  );
    isEqOpen = settings.value("isEqOpen",true).toBool();
    if(isEqOpen){
        ui->checkPoint->setVisible(true);
    }else{
        ui->checkPoint->setVisible(false);
    }

    pressTimer=new QTimer();
    holdTime=0;
    connect(pressTimer,SIGNAL(timeout()),this,SLOT(pressTimeOut()));

}

SettingsPage::~SettingsPage()
{
    delete ui;
    delete pressTimer;

}

void SettingsPage::pressTimeOut(){
    holdTime++;
    if(holdTime>=4){ //to eqSettings page
        //qDebug()<<"pressTimeOut";
        pressTimer->stop();
        EqSettingsPage  *eqPage=new EqSettingsPage(this,music);//新建子界面
        eqPage->show();
    }
}

void SettingsPage::on_back_clicked()
{
    close();
}

void SettingsPage::on_wifiSettings_clicked()
{
    WifiPage *wifi=new WifiPage(this);//新建子界面
    wifi->show();
    //delete wifi;
}

void SettingsPage::on_eqSettings_pressed()
{
    //qDebug()<<"onPress";
    pressTimer->start(1000);

}

void SettingsPage::on_eqSettings_released()
{
    //qDebug()<<"on_eqSettings_released";
    if(holdTime<2){
        isEqOpen=!isEqOpen;

        if(isEqOpen){
            ui->checkPoint->setVisible(true);
            music->eqOpenEq();
            music->adjustVolume(true);
            settings.setValue("isEqOpen",true);
        }else{
            //emit closeEq();
            ui->checkPoint->setVisible(false);
            music->eqCloseEq();
            music->adjustVolume(false);
            settings.setValue("isEqOpen",false);
        }
    }
    pressTimer->stop();
    holdTime=0;
}

