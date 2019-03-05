#include "wifipage.h"
#include "ui_wifipage.h"
#include <QDebug>
#include <QSettings>
//#include "qtvirtualkeyboard"
//#include "VirtualKeyboard 2.1"


//linx3无中文

WifiPage::WifiPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WifiPage)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(QRect(0, 0, 540, 960));//可设置窗口显示的方位与大小
    ui->dialog->setVisible(false);
    ui->deleteDialog->setVisible(false);

    ui->nameEdit->setPlaceholderText("请输入热点名称");//name of the hotspot
    ui->pswEdit->setPlaceholderText("请输入密码");//password of the hotspot
    ui->deleteWifi->setStyleSheet("QPushButton{color: white;"
                                  "background-color:""#2a2c31"";"
                                  "border-radius: 5px;"
                                  "border-style: flat;}"
                                  "QPushButton:pressed{"
                                  "background-color:rgb(24, 25, 27);"
                                  "border-style: flat; }");
    ui->cancelDele->setStyleSheet("QPushButton{color: white;"
                                  "background-color:""#2a2c31"";"
                                  "border-radius: 5px;"
                                  "border-style: flat;}"
                                  "QPushButton:pressed{"
                                  "background-color:rgb(24, 25, 27);"
                                  "border-style: flat; }");
    ui->commitButton->setStyleSheet("QPushButton{color: white;"
                                    "background-color:""#2a2c31"";"
                                    "border-radius: 5px;"
                                    "border-style: flat;}"
                                    "QPushButton:pressed{"
                                    "background-color:rgb(24, 25, 27);"
                                    "border-style: flat; }");
    ui->cancelButton->setStyleSheet("QPushButton{color: white;"
                                    "background-color:""#2a2c31"";"
                                    "border-radius: 5px;"
                                    "border-style: flat;}"
                                    "QPushButton:pressed{"
                                    "background-color:rgb(24, 25, 27);"
                                    "border-style: flat; }");
    ui->label->setStyleSheet("border-radius: 5px;"
                             "background-color: rgb(37, 38, 43);");
    ui->nameEdit->installEventFilter(this);
    ui->pswEdit->installEventFilter(this);
    ui->nameEdit->setStyleSheet( "color: black;"
                                 "background-color: ""#f2f2f2"";"
                                 "border-radius: 5px;");
    ui->pswEdit->setStyleSheet( "color: black;"
                                "background-color: ""#f2f2f2"";"
                                "border-radius: 5px;");
    ui->back->setStyleSheet(
                "QPushButton:pressed{"
                "background-color:rgb(37, 38, 43);"
                "border-style: flat; }"
                );
    ui->addWifi->setStyleSheet(
                "QPushButton:pressed{"
                "background-color:rgb(37, 38, 43);"
                "border-style: flat; }"
                );
    lastWifi= settings.value("lastWifi",-1).toInt();
    qDebug()<<"-----the last wifi is"<<QString::number(lastWifi);
    wifiInfo=new WifiInfo();
    connect(wifiInfo,
            SIGNAL(readWifiOver(QStringList,QStringList)),this,
            SLOT(onReadWifiOver(QStringList,QStringList)));
    connect(ui->listWidget,
            SIGNAL(deleteSignal(int)),this,
            SLOT(showDelDialog(int)));
    connect(wifiInfo,SIGNAL(wifiConnected()),this,SLOT(onWifiConnected()));
    wifiInfo->readWifiList();
    wifiInfo->checkWifiStatus();
    connectedName=wifiInfo->getCurrentWifiName();
    connectedIp=wifiInfo->getCurrentWifiIp();

}

WifiPage::~WifiPage()
{

    qDebug()<<"~~WifiPage";
    delete wifiInfo;
    delete ui;
    if(widget!=NULL){
      //  delete widget;
    }
    if(nitem!=NULL){
      //  delete nitem;
    }
    // delete keybod;
}
void WifiPage::onReadWifiOver(QStringList wifis, QStringList psws){
    // qDebug()<<"onReadWifiOver)))";
    this->wifis = wifis;
    this->psws = psws;
    //qDebug()<<"-------wifis.size()="<<QString::number(wifis.size());
    for(int i=0;i<wifis.size();i++){
        widget=new WifiItem();
        nitem = new QListWidgetItem(ui->listWidget);
        if(wifis.at(i)==connectedName){
            widget->setText(wifis.at(i)+"   已连接  ip:"+connectedIp);
        }else{
            if(i==lastWifi){
                widget->setText(wifis.at(i)+"   已配置");
            }else{
                widget->setText(wifis.at(i));
            }
        }
        nitem->setSizeHint(QSize(540,66));
        nitem->setFlags(nitem->flags() & ~Qt::ItemIsSelectable);
        ui->listWidget->addItem(nitem);
        ui->listWidget->setItemWidget(nitem,widget);
    }
    Labels = ui->listWidget->findChildren<QLabel*>("itemText");
    if(Labels.size()>lastWifi&&lastWifi>=0)
    {
        resetItemColor(lastWifi,lastWifi);
    }
}
void WifiPage::onWifiConnected(){
    connectedName=wifiInfo->getCurrentWifiName();
    connectedIp=wifiInfo->getCurrentWifiIp();
    for(int i=0;i<wifis.size();i++){
        if(wifis.at(i)==connectedName){
            Labels.at(i)->setText(wifis.at(i)+"   已连接  ip:"+connectedIp);
        }
    }
}
void WifiPage::showDelDialog(int index){
    delIndex=index;
    ui->deleteDialog->setVisible(true);
}
void WifiPage::resetItemColor(int lastIndex,int currentIndex){
    if(lastIndex>=0&&lastIndex<Labels.size()){
        Labels.at(lastIndex)->setStyleSheet("color:#666666;");  //adaeb8
    }
    Labels.at(currentIndex)->setStyleSheet("color:#ffffff;");
}
bool WifiPage::showKeyPad(QLineEdit *line)
{

    /*
     *  Keypad object is by default moved to 0,392
     *  was built for 800x600 resolution
     *
     * */
    line->removeEventFilter(this);
    keybod=new Keypad(line,this);
    keybod->move(0,733);
    // k->setStyleSheet("background-image: url(:/800x600/Touch-Keyboard.png)");
    if(keybod->exec() == QDialog::Accepted)
        line->installEventFilter(this);
    return true;
}
bool WifiPage::eventFilter(QObject *obj, QEvent *e)
{
    //     拦截关于此控件的所有事件，
    //     当事件是点击时显示键盘，其他事件必须返回false，
    //     否则都会被拦截，设置style等等都不会生效；
    if(obj==ui->nameEdit){
        if(e->type()==QEvent::MouseButtonPress){
            showKeyPad(ui->nameEdit);
        }
        return false;
    }
    if(obj==ui->pswEdit){
        if(e->type()==QEvent::MouseButtonPress){
            showKeyPad(ui->pswEdit);
        }
        return false;
    }
}
void WifiPage::on_back_clicked()
{


    close();
    // hide();

}

void WifiPage::on_addWifi_clicked()
{

    ui->dialog->setVisible(true);
}

void WifiPage::on_listWidget_clicked(const QModelIndex &index)
{


    int i=index.row();
   // qDebug()<<"-----------i  "<<QString::number(i);
   // qDebug()<<"labels.size "<<QString::number(Labels.size());
    if(lastWifi==i){
        return;
    }
    wifiInfo->connetWifi(wifis.at(i),psws.at(i));
    resetItemColor(lastWifi,i);
    Labels.at(i)->setText(wifis.at(i)+"   已配置");
    if(lastWifi>=0&&lastWifi<Labels.size()){
        Labels.at(lastWifi)->setText(wifis.at(lastWifi));
    }
    lastWifi=i;
    settings.setValue("lastWifi",i);
}

void WifiPage::on_listWidget_pressed(const QModelIndex &index)
{

}

void WifiPage::on_cancelDele_clicked()
{
    ui->deleteDialog->setVisible(false);
}
//删除wifi
void WifiPage::on_deleteWifi_clicked()
{
    wifiInfo->delWifi(wifis.at(delIndex),psws.at(delIndex));
    QListWidgetItem *item= ui->listWidget->takeItem(delIndex);
    ui->listWidget->removeItemWidget(item);
    wifis.removeAt(delIndex);
    psws.removeAt(delIndex);
    //  Labels = ui->listWidget->findChildren<QLabel*>("itemText");
    Labels.removeAt(delIndex);
    ui->deleteDialog->setVisible(false);
    delete item;
    if(delIndex==lastWifi){
        lastWifi=-1;
    }
    if(delIndex<lastWifi){
        lastWifi--;
    }
    settings.setValue("lastWifi",lastWifi);
}

void WifiPage::on_cancelButton_clicked()
{
    ui->dialog->setVisible(false);
}
//添加wifi
void WifiPage::on_commitButton_clicked()
{
    if(ui->nameEdit->text().isEmpty()){
       qDebug()<<"name is empty";
        return;
    }
    if(ui->pswEdit->text().isEmpty()){
        qDebug()<<"psw is empty";
        return;
    }
    wifiInfo->addWifi(ui->nameEdit->text(),ui->pswEdit->text());
    ui->dialog->setVisible(false);
    wifis.append(ui->nameEdit->text());
    psws.append(ui->pswEdit->text());
    widget=new WifiItem();
    nitem = new QListWidgetItem(ui->listWidget);
    widget->setText(ui->nameEdit->text());
    nitem->setSizeHint(QSize(540,66));
    nitem->setFlags(nitem->flags() & ~Qt::ItemIsSelectable);
    ui->listWidget->addItem(nitem);
    ui->listWidget->setItemWidget(nitem,widget);
    Labels = ui->listWidget->findChildren<QLabel*>("itemText");
}

void WifiPage::on_nameEdit_selectionChanged()
{

}
