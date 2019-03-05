#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingspage.h"
#include "QFile"
#include "app/music.h"
#include "listitem.h"
#include "myhslider3.h"
#include "additem.h"
#include "keypad.h"
#include <fcntl.h>
#include <unistd.h>     /*Unix 标准函数定义*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->errorDialog->setVisible(false);
    ui->errorTips->setStyleSheet("border-radius: 5px;"
                                 "color:white;"
                                 "background-color: rgb(37, 38, 43);");

    if(!security->checkSafety()){
        ui->errorDialog->setVisible(true);
        return;
    }
    ui->switchList->setVisible(false);
    proSliderPress=false;
    volumeSliderPress=false;
    soundCardIsReady=false;
    getFileFinish=false;
    isPlaying=false;
    isFirstCall=true;

    music = new Music() ;
    readCommand = new ReadCommand();
    soundCard=new QTimer();
    saveTimer=new QTimer();
    saveTime=0;
    connect(readCommand,SIGNAL(keyPress(int)),this,SLOT(onKeyPress(int)));
    connect(music,SIGNAL(scanningUdisk(int)),this,SLOT(udiskScanning(int)));
    connect(music,SIGNAL(scanningSDCard(int)),this,SLOT(sdcardScanning(int)));
    connect(music,SIGNAL(scanUDiskFinish(int)),this,SLOT(udiskScanFinish(int)));
    connect(music,SIGNAL(scanSDCardFinish(int)),this,SLOT(sdCardScanFinish(int)));
    connect(music,SIGNAL(getFileList(QFileInfoList,int)),this,
            SLOT(onGetFileList(QFileInfoList,int)));

    connect(music,SIGNAL(songChange(int,QPixmap,QString ,QString,QString,QString,QString)),this,
            SLOT(onSongChange(int,QPixmap,QString ,QString,QString,QString,QString)));
    connect(music,SIGNAL(updateSinger(QString,int)),this,
            SLOT(onUpdateSinger(QString,int)));
    connect(music,SIGNAL(updateSongName(QString,int)),this,
            SLOT(onUpdateSongName(QString,int)));
    connect(music,SIGNAL(songDulactionChange(qint64)),this,
            SLOT(onSongDulactionChange(qint64)));
    connect(music,SIGNAL(songPositionChange(qint64)),this,
            SLOT(onSongPositionChange(qint64)));
    connect(music,SIGNAL(addSong(QString,QString)),this,
            SLOT(onAddSong(QString,QString)));
    connect(music,SIGNAL(udiskRemove(int)),this,SLOT(onDiskRemove(int)));
    connect(music,SIGNAL(sdCardRemove(int)),this,SLOT(onDiskRemove(int)));
    connect(music,SIGNAL(startTest()),this,SLOT(onEqStartTest()));

    connect(ui->volumeSlider,SIGNAL(sliderReleasedAt(int)),this,SLOT(onVolumeReleasedAt(int)));
    connect(ui->songProSlider,SIGNAL(sliderReleasedAt(int)),this,SLOT(onProReleasedAt(int)));
    connect(soundCard,SIGNAL(timeout()),this,SLOT(checkSoundCard()));
    connect(saveTimer,SIGNAL(timeout()),this,SLOT(saveSettings()));
    connect(music,SIGNAL(eqVolumeChange(int)),this,SLOT(adjustVolume(int)));
    music->startSearchUdisk();
    soundCard->start(1000);
    readCommand->startReading();

    ui->volumeSlider->setMaximum(255);
    ui->volumeSlider->setMinimum(153);
    ui->toSettingImg->setStyleSheet(
                "QPushButton:pressed{"
                "background-color:rgb(37, 38, 43);"
                "border-style: flat; }"
                );
    ui->pageUp->setStyleSheet( "QPushButton:pressed{"
                               "background-color:""#00000000"";"
                               "border-style: flat;}"
                               );
    ui->pageDown->setStyleSheet( "QPushButton:pressed{"
                                 "background-color:""#00000000"";"
                                 "border-style: flat;}"
                                 );
    QFile file(":/qss/horizontalSlider.qss");

    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    ui->volumeSlider->setStyleSheet(styleSheet);
    ui->songProSlider->setStyleSheet(styleSheet);

    defaultVolume=settings.value("volume").toInt();
    if(defaultVolume<=153){
        defaultVolume=180;

    }
    music->setVolume(defaultVolume);
    ui->volumeSlider->setValue(defaultVolume);
    isEqOpen = settings.value("isEqOpen",true).toBool();
    if(isEqOpen){
        music->eqOpenEq();
    }else{
        music->eqCloseEq();
    }
    playMode=settings.value("playMode",1).toInt();
    if(playMode==1){
        ui->playModeImg->setStyleSheet("border-image: url(:/img/icon_list.png);");
    }
    if(playMode==2){
        ui->playModeImg->setStyleSheet("border-image: url(:/img/icon_random.png);");
    }
    if(playMode==3){
        ui->playModeImg->setStyleSheet("border-image: url(:/img/icon_single.png);");
    }
    music->setPlayMode(playMode);
}


MainWindow::~MainWindow()
{
   qDebug()<<"~MainWindow------"<<QString::number(defaultVolume);

    delete ui;
    delete music;
    delete widget;
    delete nitem;
    delete readCommand;
    delete soundCard;
    delete saveTimer;
}
void MainWindow::adjustVolume(int volume){
    ui->volumeSlider->setValue(volume);
    defaultVolume=volume;
    checkSaveState();
}
void MainWindow::udiskScanning(int type){
    if(type==1){
        ui->scanning1->setText("正在扫描U盘...");
        ui->scanning1->setVisible(true);
    }
    if(type==2){
        //add animation
    }
}
void MainWindow::sdcardScanning(int type){
    if(type==1){
        ui->scanning1->setText("正在扫描SD卡...");
        ui->scanning1->setVisible(true);
    }
    if(type==2){
        //add animation
    }
}
void MainWindow::udiskScanFinish(int type){
    if(type==1){
        ui->scanning1->setText("扫描完成");
        ui->scanning1->setVisible(false);
    }
    if(type==2){
        ui->switchList->setVisible(true);
    }
}
void MainWindow::sdCardScanFinish(int type){
    if(type==1){
        ui->scanning1->setText("扫描完成");
        ui->scanning1->setVisible(false);
    }
    if(type==2){
        ui->switchList->setVisible(true);
    }
}
void MainWindow::onDiskRemove(int type){
    ui->switchList->setVisible(false);
}

void MainWindow::checkSoundCard(){
    //检查声卡准备好而且获取列表完毕则开始播放
//    QString devPath="/dev/snd/pcmC0D0p";
    QString devPath="/dev/snd/pcmC0D3p";
    char  *ch;
    QByteArray ba = devPath.toLatin1(); // must
    ch= ba.data();
    if(!soundCardIsReady){
        int fd = ::open(ch, 0);
        if (fd ==-1) {
            qDebug()<<"---------SoundCard Is Not Ready---------"<<endl;
        }else{
            ::close(fd);
            soundCardIsReady=true;
        }
    }
    if(soundCardIsReady&&getFileFinish&&!isPlaying){
        music->playSong(lastIndex);
        isPlaying=true;
        soundCard->stop();
    }
}

void MainWindow::onGetFileList(QFileInfoList infoList, int type){
    isPlaying=false;
    currentListType=type;
    if(currentListType==1){
        lastIndex= settings.value("udiskLastSong").toInt();
    }
    if(currentListType==2){
        lastIndex= settings.value("sdLastSong").toInt();
    }
    ui->listWidget->clear();
    singerLabels.clear();
    nameLabels.clear();
    indexLabels.clear();
    if(infoList.size()<=0){
        ui->listWidget->clear();
        ui->scanning1->setText("没找到音乐.");
        ui->scanning1->setVisible(true);
        ui->songName->setText("");
        ui->singerName->setText("");
        ui->songDetail->setText("");
        ui->songImg->setPixmap(QPixmap(":/img/icon_df_head.png"));
        ui->playImg->setStyleSheet("border-image: url(:/img/icon_play.png);");
        return;
    }
    ui->scanning1->setVisible(false);
    if(infoList.size()<=lastIndex){
        lastIndex=0;
    }
    getFileFinish=true;
    if(!isFirstCall){
        music->playSong(lastIndex);
        isPlaying=true;
    }
    isFirstCall=false;
    checkSoundCard();
    thread = new AddItem();
    connect(thread,SIGNAL(addItem(int,QLabel*,QLabel*,QLabel*)),
            this,SLOT(onAddItem(int,QLabel*,QLabel*,QLabel*)));
    thread->setData(infoList,ui->listWidget,lastIndex);
}
void MainWindow::onAddItem(int i,QLabel *name, QLabel *singer, QLabel *index){

    if(i==lastIndex){
        name->setStyleSheet("color:#ffffff;");
        singer->setStyleSheet("color:#ffffff;");
        index->setStyleSheet("color:#ffffff;");
    }
    nameLabels<<name;
    singerLabels<<singer;
    indexLabels<<index;
}

void MainWindow::onSongChange(int index,QPixmap headPic,
                              QString size,QString format,QString dulation,
                              QString name,QString singer
                              ){
    qApp->processEvents();
    resetItemColor(lastIndex,index);


    if(headPic.isNull()){
        ui->songImg->setPixmap(QPixmap(":/img/icon_df_head.png"));
    }else{
        ui->songImg->setPixmap(headPic);
    }
    ui->songName->setText(geteElidedText(ui->songName,name));
    ui->singerName->setText(singer.isEmpty()?"未知":singer);
    ui->songDetail->setText("大小:"+size+"    "+"格式:"+format);
    ui->playImg->setStyleSheet("border-image: url(:/img/icon_pause.png);");
    int time=dulation.toInt();
    int second=(int)(time/1000%60);
    int min= time/1000/60;
    QString s;
    if(second<10){
        s= "0"+QString::number(second);
    }else{
        s= QString::number(second);
    }
    ui->songProSlider->setMaximum(time);
    ui->songProSlider->setMinimum(0);
    ui->songProSlider->setSingleStep(1000);
    QString time2=QString::number(min)+":"+s;
    ui->duraction->setText(time2);
    int disPlayValue=ui->listWidget->verticalScrollBar()->value();
    if((disPlayValue>index)||(index-disPlayValue)>9){
        ui->listWidget->verticalScrollBar()->setValue(index);
    }
    lastIndex=index;
    ui->lastSongImg->setEnabled(true);
    ui->nextSongImg->setEnabled(true);
    checkSaveState();
}

void MainWindow::onEqStartTest(){
    isPlaying=false;
    ui->playImg->setStyleSheet("border-image: url(:/img/icon_play.png);");
}
void MainWindow::onSongPositionChange(qint64 positon){
    int second=(int)(positon/1000%60);
    QString s;
    if(second<10){
        s="0"+QString::number(second);
    }else{
        s=QString::number(second);
    }
    if(!proSliderPress){
        ui->songProSlider->setValue(positon);
    }
    ui->position->setText(QString::number((int)(positon/1000/60))
                          +":"+s);
    currentPositon=positon;
}


void MainWindow::onKeyPress(int value){
    switch(value){
    case 104:   //KEY_PAGEUP
        music->playNext();
        break;
    case 207:   //KEY_PLAY
        music->playLast();
        break;
    case 109:   //KEY_PAGEDOWN
        if(isPlaying){
            ui->playImg->setStyleSheet("border-image: url(:/img/icon_play.png);");
            music->pause(lastIndex);
            isPlaying=false;
        }else{
            ui->playImg->setStyleSheet("border-image: url(:/img/icon_pause.png);");
            music->playSong(lastIndex);
            isPlaying=true;
        }
        break;
    }
}

void MainWindow::reShow(){
    this->show();
}
void MainWindow::checkSaveState(){
    if(saveTimer->isActive()){
        saveTime=0;
    }else{
        saveTimer->start(1000);
    }
}

void MainWindow::saveSettings(){
  saveTime++;
  if(saveTime==3){
      settings.setValue("playMode",mode);
      settings.setValue("volume",defaultVolume);
      if(currentListType==1){
          settings.setValue("udiskLastSong",lastIndex);
      }else if(currentListType==2){
          settings.setValue("sdLastSong",lastIndex);
      }
      saveTimer->stop();
      saveTime=0;
  }
}

//void MainWindow::openEqSetting(){
//    music->eqOpenEq();
//    settings.setValue("isEqOpen",true);
//}
//void MainWindow::closeEqSetting(){
//    music->eqCloseEq();
//    settings.setValue("isEqOpen",false);
//}

void MainWindow::resetItemColor(int lastIndex,int currentIndex){
    if(indexLabels.size()>lastIndex){
        nameLabels.at(lastIndex)->setStyleSheet("color:#666666;");
        singerLabels.at(lastIndex)->setStyleSheet("color:#666666;");
        indexLabels.at(lastIndex)->setStyleSheet("color:#666666;");
    }
    if(indexLabels.size()>currentIndex){
        nameLabels.at(currentIndex)->setStyleSheet("color:#ffffff;");
        singerLabels.at(currentIndex)->setStyleSheet("color:#ffffff;");
        indexLabels.at(currentIndex)->setStyleSheet("color:#ffffff;");
    }

}

QString MainWindow::geteElidedText(QLabel *label, QString str)
{
    QFontMetrics fontWidth(label->font());
    int width = fontWidth.width(str);  //计算字符串宽度
    if(width>=label->width())  //当字符串宽度大于最大宽度时进行转换
    {
        str = fontWidth.elidedText(str,Qt::ElideRight,label->width());  //右部显示省略号
    }
    return str;   //返回处理后的字符串
}

void MainWindow::on_toSettingImg_clicked()
{
    SettingsPage *setting=new SettingsPage(this,music);//新建子界面
    setting->show();
}

void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    int i=index.row();
    if(i==lastIndex){
        if(isPlaying){
            music->pause(i);
            isPlaying=false;
        }else{
            music->playSong(i);
            isPlaying=true;
        }

    }else{
        music->playSong(i);
        isPlaying=true;
    }
    if(isPlaying){
        ui->playImg->setStyleSheet("border-image: url(:/img/icon_pause.png);");
    }else{
        ui->playImg->setStyleSheet("border-image: url(:/img/icon_play.png);");
    }
}

void MainWindow::on_playImg_clicked()
{  
    if(isPlaying){
        ui->playImg->setStyleSheet("border-image: url(:/img/icon_play.png);");
        music->pause(lastIndex);
        isPlaying=false;
    }else{
        ui->playImg->setStyleSheet("border-image: url(:/img/icon_pause.png);");
        music->playSong(lastIndex);
        isPlaying=true;
    }
}

void MainWindow::on_lastSongImg_clicked()
{
    ui->lastSongImg->setEnabled(false);
    ui->nextSongImg->setEnabled(false);
    music->playLast();   
}

void MainWindow::on_nextSongImg_clicked()
{
    ui->lastSongImg->setEnabled(false);
    ui->nextSongImg->setEnabled(false);
    music->playNext();
}

void MainWindow::on_playModeImg_clicked()
{
     mode = music->changeMode();
    if(mode==1){
        ui->playModeImg->setStyleSheet("border-image: url(:/img/icon_list.png);");
    }
    if(mode==2){
        ui->playModeImg->setStyleSheet("border-image: url(:/img/icon_random.png);");
    }
    if(mode==3){
        ui->playModeImg->setStyleSheet("border-image: url(:/img/icon_single.png);");
    }
    checkSaveState();

}

void MainWindow::onVolumeReleasedAt(int pos){
    music->setVolume(pos);
    defaultVolume=pos;
    checkSaveState();

}
void MainWindow::onProReleasedAt(int pos){
    music->setSongPosition(pos);
}

void MainWindow::on_switchList_clicked()
{
    thread->setRunning(false);
    if(thread->isRunning()){
        thread->quit();
        thread->wait();
        delete thread;
    }
    ui->listWidget->clear();
    singerLabels.clear();
    nameLabels.clear();
    indexLabels.clear();
    music->changeList();
}

void MainWindow::on_pageUp_clicked()
{
    ui->pageUp->setEnabled(false);
    int disPlayValue=ui->listWidget->verticalScrollBar()->value()-10;
    if(disPlayValue<0){
        disPlayValue=0;
    }
    ui->listWidget->verticalScrollBar()->setValue(disPlayValue);
    ui->pageUp->setEnabled(true);
}

void MainWindow::on_pageDown_clicked()
{
    ui->pageDown->setEnabled(false);
    int disPlayValue=ui->listWidget->verticalScrollBar()->value()+10;
    if(disPlayValue>=ui->listWidget->count()){
        disPlayValue=ui->listWidget->count()-1;
    }
    ui->listWidget->verticalScrollBar()->setValue(disPlayValue);
     ui->pageDown->setEnabled(true);
}
