#include "readudisk.h"
#include "QDebug"
#include <QObject>
#include <fcntl.h>
#include <QDir>
#include "qmmp/music.h"
#include <iostream>
#include <cstring>        // for strcat()



#include <qfileinfo.h>

ReadUdisk::ReadUdisk(){
    // music=new Music();
}
ReadUdisk::~ReadUdisk(){
}
void ReadUdisk::run(){
    while (1) {
        //path="/dev/sd?"
        for(int i=0;i<10;i++){
            QString path="/run/media/sda"+QString::number(i)+"/";
            QDir dir(path);
            if(!dir.exists()){
                // qDebug()<<"not open,path="<<path<<endl;

            }else{
                qDebug()<<"has open,path="<<path<<endl;

                //  if(music->playlist.isEmpty()){
                listFiles(path);
                setPlaylist();
                //  emit skinUdiskFinish(file_list);
                //}


                //列出dir(path)目录文件下所有文件和目录信息，存储到file_list容器
                // QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
                //列出dir(path)目录下所有子文件夹
                //                QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
                //                //进行子文件夹folder_list递归遍历，将内容存入file_list容器
                //                for(int i= 0; i != folder_list.size(); i++)
                //                {
                //                    QString name = folder_list.at(i).absoluteFilePath();
                //                    qDebug()<<"DIR>>>>>>>>>>"<<name<<endl;
                //                    // QFileInfoList child_file_list = GetFileList(name);
                //                    //  file_list.append(child_file_list);
                //                }
                //                msleep(150000);
                // return ;

            }

        }

        msleep(1500);

        //
    }

}
QFileInfoList ReadUdisk::listFiles(QString path){
    QDir dir(path);
    QDir dirFile(path);
    //dir.setFilter(QDir::Files | QDir::NoSymLinks);//设置类型过滤器，只为文件格式
    dirFile.setNameFilters(music->player->supportFormatsFilterString());//设置文件名的过滤
    file_list = dirFile.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks
                                      );
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folder_list.size(); i++)
    {
        QString name = folder_list.at(i).absoluteFilePath();
        QFileInfoList child_file_list = listFiles(name);
        file_list.append(child_file_list);
        file_list2.append(file_list);
    }
    //qDebug()<<"SIZE>>>>>>>>>>>>>="<<file_list.size()<<endl;
    for(int i=0;i<file_list.size();i++){
        qDebug()<<file_list.at(i).filePath()<<endl;
    }
    // file_list2.clear();

    return file_list;
}
void ReadUdisk::setPlaylist(){
    QStringList pathList;
    if(file_list2.size()>0){
        for(int i=0;i<file_list2.length();i++){
            pathList.operator <<(file_list2.at(i).absoluteFilePath());
        }
        music->playlist->addMedia(pathList);
        music->player->setPlaylist(music->playlist);
        qDebug("add Song!");
    }else{
        qDebug("find no songs!");
    }
    // qDebug()<<"-----SIZE=="<<QString::number(file_list2.size())<<endl;
}
