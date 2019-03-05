#ifndef ADDITEM_H
#define ADDITEM_H

#include <QObject>
#include <QThread>
#include <QListWidgetItem>
#include "listitem.h"
#include <QLabel>
#include <QFileInfoList>

class AddItem : public QThread
{
    Q_OBJECT
public:
    AddItem();
    void setData(QFileInfoList filePaths,QListWidget *widget,int playingIdx);
    void setRunning(bool running);
    void setPlayingIndex(int playingIdx);
signals:
    void addItem(int i,QLabel *name,
                 QLabel *singer,QLabel *index);
protected:
    void run();
public slots:
    //    void onTimeOut();
private:
    QFileInfoList filePaths;
    QListWidgetItem  *item;
    listItem  *widget;
    QListWidget *listWidget;
    int playingIdx;
    QList<QLabel *> nameLabels;
    QList<QLabel *> singerLabels;
    QList<QLabel *> indexLabels;
    bool running=true;
};

#endif // ADDITEM_H
