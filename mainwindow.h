#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "app/music.h"
#include "listitem.h"
#include "app/security.h"
#include "qfileinfo.h"
#include "additem.h"
#include "app/readcommand.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    // void on_pushButton_clicked();
    void reShow();
    void udiskScanning(int type);
    void sdcardScanning(int type);
    void udiskScanFinish(int type);
    void sdCardScanFinish(int type);

    void onSongChange(int index,QPixmap headPic,
                      QString size,QString format,QString dulation,
                      QString name,QString singer);

    void onGetFileList(QFileInfoList infoList,int listType);

   // void openEqSetting();
   // void closeEqSetting();

    void onSongPositionChange(qint64 positon);

    void onDiskRemove(int type);
    void onEqStartTest();
    void onAddItem(int i,QLabel *name,QLabel *singer,QLabel *index);
    void checkSoundCard();
    void saveSettings();
    void checkSaveState();
    void adjustVolume(int volume);
   // void onSoundCardReady();

    void on_toSettingImg_clicked();
    void on_listWidget_clicked(const QModelIndex &index);
    void on_playImg_clicked();
    void on_lastSongImg_clicked();
    void on_nextSongImg_clicked();
    void on_playModeImg_clicked();
    void onKeyPress(int value);

    void onVolumeReleasedAt(int pos);
    void onProReleasedAt(int pos);



    //void on_volumeSlider_sliderMoved(int position);










    void on_switchList_clicked();

    // void on_nextPage_clicked();

    void on_pageUp_clicked();


    void on_pageDown_clicked();

private:
    Ui::MainWindow *ui;
    Music *music;
    ReadCommand *readCommand;
    QList<QLabel*> nameLabels;
    QList<QLabel*> singerLabels;
    QList<QLabel*> indexLabels;
    Security *security;
    int lastIndex;
    bool isPlaying;
    AddItem *thread;
    listItem *widget;
    QListWidgetItem *nitem;
    void resetItemColor(int lastIndex,int currentIndex);
    QString geteElidedText(QLabel *label, QString str);
    bool volumeSliderPress;
    int currentPositon; //歌曲播放当前位置
    bool proSliderPress;
    QSettings settings;

    int currentListType;
    int defaultVolume;
    bool isEqOpen;
    int playMode;
    bool soundCardIsReady;
    bool getFileFinish;
    bool isFirstCall;
    int saveTime;
    int mode;

    QTimer *saveTimer;
    QTimer *soundCard;
    //QStringList songlist;


public:
    //    class MyThread : public QThread
    //    {
    //        Q_OBJECT

    //    protected:
    //        void run();
    //    public:
    //        MyThread(QObject *parent=0);
    //        ~MyThread();
    //         QStringList songlist;

    //        listItem *widget;
    //        QListWidgetItem *nitem;
    //        QList<QLabel*> nameLabels;
    //        QList<QLabel*> singerLabels;
    //        QList<QLabel*> indexLabels;

    //    };
};

#endif // MAINWINDOW_H
