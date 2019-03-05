#ifndef READUDISK_H
#define READUDISK_H
#include <QThread>
#include <QObject>
#include <qfileinfo.h>
#include "music.h"
class ReadUdisk :public QThread{
    Q_OBJECT
public:
    ReadUdisk();
    ~ReadUdisk();
    QFileInfoList file_list;
    QFileInfoList file_list2;

private:
    void run();
    int fd;
    QFileInfoList listFiles(QString path);
    Music *music;
    void setPlaylist();
Q_SIGNALS:
    void skinUdiskFinish(QFileInfoList file_list);
};
#endif // READUDISK_H
