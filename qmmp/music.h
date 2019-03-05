#ifndef MUSIC_H
#define MUSIC_H
#include <QObject>
#include "musicplayer.h"
#include "musicplaylist.h"
#include "qmmp/qmmpplugincache_p.h"
#include <qmmp/visualfactory.h>
#include <QProcess>
#include <QTimer>
#include <qfileinfo.h>
#include <QThread>
#include "musicsongtag.h"
#include "showimage.h"
#include <QImage>
#include "ffteqsetting.h"
class MusicPlayer;
class MusicPlaylist;

class Music : public QThread
{
    Q_OBJECT
public:
    //  Music();
    Music(QThread *parent= 0);
    //Music();
    ~Music();

    // Q_INVOKABLE void searchFiles(QString path);
    Q_INVOKABLE void playSong(int index);
    Q_INVOKABLE void pause(int index);
    Q_INVOKABLE QStringList getMedia();
    Q_INVOKABLE void volumeDown();
    Q_INVOKABLE void setVolume(int volume);
    Q_INVOKABLE void setSongPosition(qint64 position);
    Q_INVOKABLE void setPlayMode(int mode);
    Q_INVOKABLE void playNext();
    Q_INVOKABLE void playLast();
    Q_INVOKABLE void volumeUp();
    Q_INVOKABLE void startSearchUdisk();

    Q_INVOKABLE void eqStartTest();
    Q_INVOKABLE void eqOpenEq();
    Q_INVOKABLE void eqCloseEq();
    Q_INVOKABLE void eqSaveSetting();
    Q_INVOKABLE QList<int> eqLoadSetting();
    Q_INVOKABLE void eqApplaySetting(int index,int value);
    Q_INVOKABLE void changeList();
   // Q_INVOKABLE void getNextPage();


    QThread *scanThread;
    // MusicSongTag  tag;
    AutoEQTest *m_autoEq;
    FFTSetting *m_fft;
   // QThread *scanDetail;
    // ShowImage *showImage;
public:
    MusicPlayer *player;
    MusicPlaylist *playlist;
    //protected:
    //    QTimer searchUdiskTimer;

private:
    //void searchFiles();
    void loadPlugin();

    VisualFactory *factory;
    QProcess *cmd;
    QString program;
    QStringList arguments;

    // QProcess *vncCmd;
    QString vncProgram;
    QStringList vncArguments;
    QString udiskPath;

    //static QList<QmmpPluginCache*> *m_cache;
    int count;
    int defaultValume;
    int flag;
    int flagSd;
    bool isReadingSDCard;
    bool isReadingUdisk;

    QFileInfoList scanMusic(QString path);
    QFileInfoList songList;
    QString getUdiskPath();
    void setVolumeSilent();
    void run();
    //Music * music;
    QList<QString> m_valueEq;
    QList<QPointF> m_points;

    QString outputFileName;
    QString outputFilePath;
    QString musicPath;
    QString musicPathSD;
    bool isEmptyUdisk;
    bool isEmptySDCard;
    bool sdCardHasInsert;
    bool uDiskHasInsert;
    void searchUdisk();
    void scanUdisk();
    void searSDCard();
    void scanSDCard();
    void updatePlayerList(QFileInfoList songList,int listType);
    QFileInfoList uDiskSongs;
    QFileInfoList sdCardSongs;
    int currentSongList;
    int currentPage;

    QStringList uNames;
    QStringList uSingers;
//    QStringList uSizes;
//    QStringList uBits;
//    QStringList uFormat;
     QStringList uPaths;

    QStringList sdNames;
    QStringList sdSingers;
    QStringList sdSizes;
    QStringList sdBits;
    QStringList sdFormat;
    QStringList sdPaths;




Q_SIGNALS:
    void songChange(int index,QImage headPic,bool change,QString samplingRate
                    ,QString size,QString format);
    void songDulactionChange(qint64 duration);
    void songPositionChange(qint64 duration);
    //type：提示类型 1：在全屏显示 2：横条显示
    void scanningUdisk(int type);
    //type：提示类型 1：在全屏显示 2：横条显示
    void scanningSDCard(int type);
    void scanFinished(QStringList nameList,QStringList singerList,
                      QStringList sizeList,QStringList bitList);
//    void updateSongList(QStringList nameList,QStringList singerList,
//                        QStringList sizeList,QStringList bitList,
//                        QStringList formatList,
//                        int listType);
//    void updateSongList(QStringList nameList,
//                        int listType);
    void updateSongList(QStringList nameList,QStringList singerList,
                        int listType);
    void updateSinger(QString singerName,int index);
    void updateSongName(QString songName,int index);
    void changingList();
   // void nextPage(QStringList nameList,QStringList singerList);
    //type：提示类型 1：在全屏显示 2：横条显示
    void scanSDCardFinish(int type);
    void scanUDiskFinish(int type);
    //type：提示类型 1：在全屏显示 2：横条显示
    void udiskRemove(int type);
    //type：提示类型 1：在全屏显示 2：横条显示
    void sdCardRemove(int type);
    void eqDoneTest();
    void eqError();


public slots:
    void songChanged(int index);
    void songDulactionChanged(qint64 duration);
    void songPositionChanged(qint64 duration);

    void eqDonePlay(MusicPlayer::State newState);
    void eqDoneCalculate(QList<QString> values);
    void eqCalculateError();
    void test();  //useless
};

#endif // MUSIC_H

