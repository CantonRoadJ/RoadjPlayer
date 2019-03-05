#include "additem.h"
#include <QDebug>
#include <QCoreApplication>
#include "app/musicsongtag.h"
#include <QFile>


AddItem::AddItem(){

}
void AddItem::run(){

    for(int i=0;i<filePaths.size();i++){
        if(!running){
            break;
        }
        item = new QListWidgetItem(listWidget);
        widget=new listItem;

        QFile file;
        file.setFileName(filePaths.at(i).filePath());
        if (!file.exists()){
            qDebug()<<"----File Not Found, Disk Has Remove??----";
            break;
        }
        MusicSongTag tag;
        tag.read(filePaths.at(i).filePath());
        QString songName = filePaths.at(i).fileName();
        if(tag.getTitle().isNull()||tag.getTitle().length()==0){
            songName=songName.left(songName.indexOf("."));
        }else{
            songName =tag.getTitle();
        }
        widget->setIndex(i+1);
        widget->setSongName(songName);
        QString singer=tag.getArtist().isEmpty()?"未知":tag.getArtist();

        widget->setSinger(singer);
        item->setSizeHint(QSize(540,66));
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

        this->listWidget->addItem(item);
        this->listWidget->setItemWidget(item,widget);

        QCoreApplication::processEvents();
        emit addItem(i,widget->getNameLabel(),
                     widget->getSingerLabel(),
                     widget->getIndexLabel());

    }
}
void AddItem::setData(QFileInfoList filePaths,QListWidget *widget,
                      int playingIdx){
    this->filePaths=filePaths;
    this->listWidget=widget;
    this->playingIdx =playingIdx;
    run();
}
void AddItem::setRunning(bool running){
    this->running=running;
}
void AddItem::setPlayingIndex(int playingIdx){
    this->playingIdx=playingIdx;
}
