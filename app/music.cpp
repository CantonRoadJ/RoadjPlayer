#include "music.h"
#include "musicplayer.h"
#include "musicplaylist.h"
#include <qdir.h>
#include "QDebug"
#include "qmmp/include/soundcore.h"
#include "readudisk.h"
#include <QTextCodec>
#include "musicsongtag.h"
#include <fft/include/utils.h>
#include <fft/include/fftw.h>
#include <fcntl.h>
#include <unistd.h>     /*Unix 标准函数定义*/
#include <errno.h>
#include <QException>
#include <app/readcommand.h>
#include <QMetaType>


Music::Music()
{
    flag = 0;
    flagSd=0;
    isReadingSDCard=true;
    isReadingUdisk=true;
    currentPage=0;
    currentSongList=1; //当前音乐列表标识:1、u盘 ；2、sd卡
    isEmptyUdisk=false;
    isEmptySDCard=false;
    uDiskHasInsert=false;
    sdCardHasInsert=false;

    outputFileName="/home/root/music/output/outData.txt";
    outputFilePath="/home/root/music/output/";
    musicPath="/run/media/sda";
    // musicPath="/media/jia/UDISK";
    musicPathSD="/run/media/mmcblk0p1";
    qRegisterMetaType< QFileInfoList> ("QFileInfoList");

    //    outputFileName="/home/jia/公共的/outData.txt";
    //    outputFilePath="/home/jia/公共的/";
    //    musicPath="/home/jia/音乐";
    //    musicPath="/media/jia/LQY";
    playMode=1;
    player = new MusicPlayer(this);
    playlist = new MusicPlaylist(this);
    playlist->setPlaybackMode(MC_PlayListLoop);
    cmd = new QProcess;
    defaultValume=200;//(255/100*60)
    program = "amixer";
    arguments << "cset"<<"numid=1,iface=MIXER,name='DAC Playback Volume" << QString::number(defaultValume);
    //    cmd->start(program,arguments);
    m_fft = new FFTSetting();
    m_autoEq = new AutoEQTest();
    m_autoEq->fftPlayer=player;
    m_autoEq->fftPlaylist=playlist;
    m_valueEq=m_autoEq->loadEQData(outputFileName);
    connect(playlist,SIGNAL(currentIndexChanged(int)),this,SLOT(songChanged(int)));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(songDulactionChanged(qint64)));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(songPositionChanged(qint64)));
    connect(player,SIGNAL(stateChanged(MusicPlayer::State)),SLOT(eqDonePlay(MusicPlayer::State)));
    connect(m_autoEq,SIGNAL(calculateFinish(QList<QString>)),this, SLOT(eqDoneCalculate(QList<QString>)));
    connect(m_autoEq,SIGNAL(calculateError()),this,SLOT(eqCalculateError()));
}
Music::~Music(){

    delete player;
    delete playlist;
    delete m_fft;
    delete m_autoEq;
    delete cmd;
    // system("killall x11vnc");
}

void Music::test(){
    // qDebug()<<"staFFFFFFFFFFFFf"<<endl;
}
QStringList Music::getMedia(){

    return playlist->m_mediaList;
}
void Music::playNext(){
    if(!playlist->isEmpty() ){
        player->pause();
        player->playNext();
        player->play();
    }
}
void Music::playLast(){
    if(!playlist->isEmpty() ){
        player->pause();
        player->playPrivious();
        player->play();
    }
}
void Music::playSong(int index){
    if(!playlist->isEmpty() ){
        //        qDebug()<<"-----m_currentIndex----"<<playlist->m_currentIndex<<endl;
        //        if(playlist->m_currentIndex!=index){
        playlist->m_currentIndex=index;
        songChanged(index);
        //    }
        player->play();

    }
}
void Music::pause(int index){

    if(!playlist->isEmpty()){
        if(playlist->m_currentIndex==index){
            player->pause();
        }else{
            playlist->m_currentIndex=index;
            player->play();
        }
    }
}
void Music::songChanged(int index){
    MusicSongTag tag;
    QPixmap pic;
    QString size;
    QString format;
    QString path;
    QString dulation;
    QString songName ;
    QString singer;
    if(currentSongList==1){
        path=uDiskSongs.at(index).filePath();
        format=uDiskSongs.at(index).suffix();
        songName = uDiskSongs.at(index).fileName();
    }else if(currentSongList==2){
        path=sdCardSongs.at(index).filePath();
        format=sdCardSongs.at(index).suffix();
        songName = sdCardSongs.at(index).fileName();
    }
    tag.read(path);
    pic=tag.getCover();
    size=tag.getSize();

    if(tag.getTitle().isNull()||tag.getTitle().length()==0){
        songName=songName.left(songName.indexOf("."));
    }else{
        songName =tag.getTitle();
    }
    singer= tag.getArtist();
    dulation=tag.getLengthString();

    emit songChange(index,pic,size,format,dulation,songName,singer);
}
void Music::songDulactionChanged(qint64 duration){
    //qDebug()<<"-----songDulactionChanged-----"<<duration;
    emit songDulactionChange(duration);
}
void Music::songPositionChanged(qint64 duration){
    emit songPositionChange(duration);
}
void Music::setVolume(int volume){
    if(volume<=153){
        defaultValume=0;
    }else{
        defaultValume=volume;
    }
    arguments.removeLast();
    arguments << QString::number(defaultValume);
    cmd->waitForFinished(3000);
    cmd->start(program,arguments);
}
//void Music::setVolumeSilent(){
//    arguments.removeLast();
//    arguments << QString::number(0);
//    cmd->start(program,arguments);
//}
void Music::setPlayMode(int mode){
    if(mode==1){
        playlist->setPlaybackMode(MC_PlayListLoop);
    }
    if(mode==2){
        playlist->setPlaybackMode(MC_PlayRandom);
    }
    if(mode==3){
        playlist->setPlaybackMode(MC_PlayOneLoop);
    }

}
int Music::changeMode(){
    if(playMode==1){
        playlist->setPlaybackMode(MC_PlayRandom);
        playMode=2;
        return playMode;
    }
    if(playMode==2){
        playlist->setPlaybackMode(MC_PlayOneLoop);
        playMode=3;
        return playMode;
    }
    if(playMode==3){
        playlist->setPlaybackMode(MC_PlayListLoop);
        playMode=1;
        return playMode;
    }
    return 1;
}

void Music::setSongPosition(qint64 position){
    if(currentSongList==1){
        if(player!=NULL&&uDiskSongs.size()>0){
            player->setPosition(position);
        }
    }else if(currentSongList==2){
        if(player!=NULL&&sdCardSongs.size()>0){
            player->setPosition(position);
        }
    }
}
void Music::volumeDown(){
    if(defaultValume>0){
        defaultValume-=255/100;
    }

    arguments.removeLast();
    arguments << QString::number(defaultValume);
    if(!cmd->waitForFinished(3000))
        cmd->start(program,arguments);
}
void Music::volumeUp(){
    if(defaultValume<255){
        defaultValume+=255/100;
    }
    arguments.removeLast();
    arguments << QString::number(defaultValume);
    if(!cmd->waitForFinished(3000))
        cmd->start(program,arguments);
}
QString Music::getUdiskPath(){
    for(int i=0;i<10;i++){
        QString tempPath =musicPath+QString::number(i);
        QDir dir(tempPath);
        if(dir.exists()){
            QString devPath="/dev/sda"+QString::number(i);
            char  *ch;
            QByteArray ba = devPath.toLatin1(); // must
            ch= ba.data();
            int fd = ::open(ch, 0);
            if (fd ==-1) {
                // qDebug()<<"---------Dev Did Not Exists---------"<<endl;
                return "";
            }else{
                ::close(fd);
            }
            udiskPath=tempPath;
            return udiskPath;
        }
    }
    return "";
    //    udiskPath=musicPath;
    //    QDir dir(udiskPath);
    //    if(dir.exists()){
    //        return udiskPath;
    //    }else{
    //        return "";
    //    }
}
void Music::startSearchUdisk(){
    start();
}
void Music::run(){
    while (1) {
        searchUdisk();
        searSDCard();
        msleep(1000);
    }
}

//寻找U盘
void Music::searchUdisk(){

    if(flag == 2)
    {
        if(currentSongList==1){
            emit udiskRemove(1);
            changeList();
        }else{
            emit udiskRemove(2);
        }
        uNames.clear();
        uSingers.clear();
        uDiskSongs.clear();
        uPaths.clear();
        if(!isReadingUdisk){
            //qDebug()<<"----------isReading=false";
            char  *chUmount;
            QString orderUmount="umount "+udiskPath+"/";
            QByteArray ba=orderUmount.toLatin1();
            chUmount=ba.data();
            FILE *fp;
            // char buf[200] = {0};
            if((fp = popen(chUmount, "r")) == NULL){
                //  qDebug()<<"----command Error!!!----";
            }else{
                // qDebug()<<"----command Success!!!----";
            }
            char  *chRM;
            QString orderRM="rm -r "+udiskPath;
            QByteArray baRM=orderRM.toLatin1();
            chRM=baRM.data();
            system(chRM);
            isEmptyUdisk=false;
            uDiskHasInsert=false;
            flag = 0;
        }else{
            // qDebug()<<"--------------isReadingUdisk=true";
        }
    }

    if(!getUdiskPath().isEmpty()){

        if(isEmptyUdisk){
            return;
        }
        if(uDiskSongs.size()<=0){
            uDiskHasInsert=true;
            flag = 1;
            scanUdisk();
        }
    }else{
        if(uDiskSongs.size()>0||isEmptyUdisk||uDiskHasInsert){
            player->pause();
            player->stop();
            flag = 2;
        }
    }
}
//扫描U盘
void Music::scanUdisk(){
    if(currentSongList!=2){
        currentSongList=1;
        emit scanningUdisk(1);
    }else{
        emit scanningUdisk(2);
    }
    uDiskSongs.clear();
    uDiskSongs = scanMusic(udiskPath);


    updatePlayerList(uDiskSongs,1);
    if(uDiskSongs.size()>0)
    {
        isEmptyUdisk=false;
    }else{
        isEmptyUdisk=true;
    }
}
void Music::searSDCard(){
    QDir dirSD(musicPathSD);
    if(dirSD.exists()){
        QString devPath="/dev/mmcblk0p1";
        char  *ch;
        QByteArray ba = devPath.toLatin1(); // must
        ch= ba.data();
        int fd = ::open(ch, 0);
        if (fd ==-1) {
            if(sdCardSongs.size()>0||isEmptySDCard||sdCardHasInsert){
                flagSd=2;
            }
        }else{
            ::close(fd);
            if(isEmptySDCard){
                return;
            }
            if(sdCardSongs.size()<=0){
                flagSd=1;
                sdCardHasInsert=true;
                scanSDCard();
            }
        }
    }else{
        if(sdCardSongs.size()>0||isEmptySDCard||sdCardHasInsert){
            flagSd=2;
        }
    }
    if(flagSd==2){
        if(currentSongList==2){
            emit sdCardRemove(1);
            changeList();
        }else{
            emit sdCardRemove(2);
        }
        sdNames.clear();
        sdSingers.clear();
        sdSizes.clear();
        sdBits.clear();
        sdCardSongs.clear();
        sdPaths.clear();
        if(!isReadingSDCard){
            char  *chUmount;
            QString orderUmount="umount "+musicPathSD+"/";
            QByteArray ba=orderUmount.toLatin1();
            chUmount=ba.data();
            system(chUmount);
            char  *chRM;
            QString orderRM="rm -r "+musicPathSD;
            QByteArray baRM=orderRM.toLatin1();
            chRM=baRM.data();
            system(chRM);
            isEmptySDCard=false;
            sdCardHasInsert=false;
            flagSd = 0;
        }

    }
}
void Music::scanSDCard(){

    if(sdCardHasInsert&&!uDiskHasInsert){
        currentSongList=2;
        emit scanningSDCard(1);
    }else{
        emit scanningSDCard(2);
    }
    sdCardSongs.clear();
    sdCardSongs = scanMusic(musicPathSD);


    updatePlayerList(sdCardSongs,2);
    if(sdCardSongs.size()>0)
    {
        isEmptySDCard=false;
    }else{
        isEmptySDCard=true;
    }
}
//根据路径扫描音乐（U盘/SD卡）
QFileInfoList Music::scanMusic(QString path){
    // qDebug()<<"----Scanning Path---"<<path;
    QDir dir(path);
    QDir dirFile(path);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);//设置类型过滤器，只为文件格式
    dirFile.setNameFilters(player->supportFormatsFilterString());//设置文件名的过滤
    QFileInfoList file_list = dirFile.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    if(folder_list.size()<=0){
        // qDebug()<<"----folder_list.size()<=0---";
        return file_list;
    }
    for(int i = 0; i != folder_list.size(); i++)
    {
        QString name = folder_list.at(i).absoluteFilePath();

        QFileInfoList child_file_list = scanMusic(name);
        file_list.append(child_file_list);
    }
    return file_list;
}

//type：1：U盘 2：SD卡
void Music::updatePlayerList(QFileInfoList songList,int listType){

    if(listType==1){
        isReadingUdisk=true;
        uPaths.clear();
        uNames.clear();
        uSingers.clear();
        for(int i = 0; i < songList.size(); ++i)
        {
            uPaths <<songList.at(i).filePath();
        }

        if(currentSongList==1){
            playlist->clear();
            playlist->addMedia(uPaths);
            player->setPlaylist(playlist);
            emit getFileList(songList,1);
            emit scanUDiskFinish(1);
        }else{
            emit scanUDiskFinish(2);
        }
        isReadingUdisk=false;
    }
    if(listType==2){
        isReadingSDCard=true;
        sdPaths.clear();
        sdNames.clear();
        sdSingers.clear();
        for(int i = 0; i < songList.size(); ++i)
        {
            sdPaths <<songList.at(i).filePath();
        }

        if(currentSongList==2){
            playlist->clear();
            playlist->addMedia(sdPaths);
            player->setPlaylist(playlist);
            emit getFileList(songList,2);
            emit scanUDiskFinish(1);
        }else{
            emit scanUDiskFinish(2);
        }
        isReadingSDCard=false;
    }
}
void Music::changeList(){
    player->pause();
    player->stop();
    playlist->clear();
    emit changingList();
    if(currentSongList==1){
        currentSongList=2;
        playlist->addMedia(sdPaths);
        player->setPlaylist(playlist);

        if(sdPaths.size()<=0){
            currentSongList=0;
        }
        // sdCardSongs
        emit getFileList(sdCardSongs,2);

    }else if(currentSongList==2){
        currentSongList=1;
        playlist->addMedia(uPaths);
        player->setPlaylist(playlist);

        if(uPaths.size()<=0){
            currentSongList=0;
        }
        emit getFileList(uDiskSongs,1);

    }else if(currentSongList==0){
        if(sdPaths.size()>0){
            playlist->addMedia(sdPaths);
            player->setPlaylist(playlist);
            emit getFileList(sdCardSongs,2);
            currentSongList=2;
            return;
        }
        if(uPaths.size()>0){
            playlist->addMedia(uPaths);
            player->setPlaylist(playlist);
            emit getFileList(uDiskSongs,1);
            currentSongList=1;
            return;
        }
    }
}

void Music::eqOpenEq(){
    qDebug()<<"-----eqOpenEq---"<<endl;

    if(m_valueEq.size()>0){
        EqSettings settings(EqSettings::EQ_BANDS_31);
        settings.setEnabled(true);
        settings.setPreamp(0);
        for(int i=0;i<m_valueEq.size();i++){
            settings.setGain(i,m_valueEq[i].toInt());
        }
        if(player!=NULL){
            if(!playlist->isEmpty()){
                player->pause();
            }
            player->setEqSettings(settings);
            if(!playlist->isEmpty()){
                player->play();
            }
        }else{
            //  qDebug()<<"-----player is NULL---"<<endl;
        }

    }else{
        for(int i = 0; i < 31; i++)
        {
            m_valueEq.append("0");
        }
    }
}
void Music::eqCloseEq(){
    EqSettings settings(EqSettings::EQ_BANDS_31);
    for(int i=0;i<EqSettings::EQ_BANDS_31;i++){
        settings.setGain(i,0);
    }
    settings.setEnabled(false);
    settings.setPreamp(0);
    if(!playlist->isEmpty()){
        player->pause();
    }
    player->setEqSettings(settings);
    if(!playlist->isEmpty()){
        player->play();
    }
}
void Music::adjustVolume(bool isOpen){
    if(isOpen){
        defaultValume+=24;
        if(defaultValume>255){
            defaultValume=255 ;
        }
    }else{
        defaultValume-=24;
        if(defaultValume<=153){
            defaultValume=177 ;
        }
    }
    setVolume(defaultValume);
    emit eqVolumeChange(defaultValume);
}
void Music::eqStartTest(){
    //测试时把音量调到最大
    arguments.removeLast();
    arguments << QString::number(255);
    cmd->waitForFinished(3000);
    cmd->start(program,arguments);
    eqCloseEq();
    m_autoEq->autoTest();
    emit startTest();
}
void Music::eqSaveSetting(){
    QString path=outputFilePath;
    if(m_autoEq->saveEQData(path,m_valueEq)){
    }else{
        qDebug()<<"--------save failed---------";
    };
}
QList<int> Music::eqLoadSetting(){
    QList<int> list;
    for(int i=0;i<m_valueEq.size();i++){
        list.append(atoi(m_valueEq[i].toStdString().c_str())) ;
    }
    return list;

}
void Music::eqDonePlay(MusicPlayer::State sta){
    //  播放停止跳转到此方法
    if(player->isTesting){//如果播放器正在测试，则去计算，调试
        m_autoEq->donePlay(sta);
    }
}

void Music::eqDoneCalculate(QList<QString> values){
    qDebug()<<"----eqDoneCalculate";
    m_points = m_autoEq->getPoints();
    m_valueEq.clear();
    m_valueEq=values;
    player->isTesting=false;
    EqSettings settings(EqSettings::EQ_BANDS_31);
    settings.setEnabled(true);
    settings.setPreamp(0);
    for(int i=0;i<m_valueEq.size();i++){
        settings.setGain(i,atoi(m_valueEq[i].toStdString().c_str()));
        //settings.setGain(i,20);
    }
    setVolume(defaultValume); //测试完成还原音量
    eqSaveSetting();

    emit eqDoneTest(m_points);

}
void Music::eqCalculateError(){
    qDebug()<<"---eqCalculateError---";
    qDebug()<<"defvalue----"<<QString::number(defaultValume);
    setVolume(defaultValume); //测试完成还原音量
    //is here wrong??18:30
    player->isTesting=false;
    m_points = m_autoEq->getPoints();


    //emit eqDoneTest(m_points);
    emit eqError(m_points);
}
void Music::eqApplaySetting(int index,int value){
    EqSettings settings(EqSettings::EQ_BANDS_31);
    settings.setEnabled(true);
    if(index==-1){
        settings.setPreamp(value);
    }else{
        settings.setGain(index,value);
        m_valueEq.replace(index, QString::number(value));
    }
    player->setEqSettings(settings);
}
FFTSetting::FFTSetting(QObject *parent) : QObject(parent)
{
    m_file = 0;
    m_fft = new fftw(this);
    connect(m_fft,SIGNAL(fftComplete(FrequencySpectrum&)),this,SLOT(getData(FrequencySpectrum&)));
}

FFTSetting::~FFTSetting()
{
    m_fft->fftClose();
    delete m_fft;
}

bool FFTSetting::loadFile(QString filename)
{
    if(isWavFile(filename))
    {
        m_file = fopen(filename.toLocal8Bit().data(),"r");
        readHeader();
        return true;
    }
    return false;
}

void FFTSetting::readHeader()
{
    struct wavfile_header header;
    if(fread(&header,1,sizeof(header),m_file) > 0)
    {
        m_length = header.num_channels * header.sample_rate;
        m_spectrum = m_length;
    }
}

QList<QPointF> FFTSetting::getPoints()
{
    return m_points;
}

bool FFTSetting::isWavFile(const QString filename)
{
    QString type = filename.split(".").last();
    if(type == "wav")
        return true;
    return false;
}

void FFTSetting::setData()
{
    short data[m_length];
    fftw_complex in[m_length];
    while((fread(data,sizeof(short),m_length,m_file)) > 0)
    {
        for (int i = 0; i < m_length; i++) {
            double hannWindow = 0.5 * (1 - cos((2 * M_PI * i) / (m_length - 1)));
            in[i][REAL] = hannWindow * in[i][REAL];
        }

        for(int i = 0; i < m_length; i++)
        {
            in[i][REAL] = data[i];
            in[i][IMAG] = 0;
        }
        m_fft->initialize(m_length,in);
        m_fft->fftStart();
    }
    qDebug() << "---setData() count = " << m_count;
    fclose(m_file);
    emit spectrumComplete(m_points);
}

void FFTSetting::getData(FrequencySpectrum &spectrum)
{
    m_spectrum = spectrum;
    FrequencySpectrum::const_iterator x = m_spectrum.begin();
    const FrequencySpectrum::const_iterator end = m_spectrum.end();
    qreal value = 0.0;
    qreal frequency = 0.0;
    QPointF point;
#if 0
    for (; x!=end; ++x)
    {
        //value = qMax(value,x->amplitude);
        if(value < (x->amplitude) && (x->frequency) <= 20000 && (x->frequency) > 10){
            //qDebug() << x->amplitude;
            value = qMax(value,x->amplitude);
            frequency = x->frequency;
        }
    }
    //qDebug() << "count = " << m_count;
    //m_textStream << "count " << m_count << "\n";
    if(frequency <= 20000)
    {
        value = qLn(value);
        point.setX(frequency);
        point.setY(value);
        m_points.append(point);
        qDebug() << point;
        m_textStream << frequency << "\t"
                     << value << "\t" << "\n";
    }

#endif
    for (; x!=end; ++x)
    {
        switch (m_count) {
        case 0:
            if(value < (x->amplitude) && (x->frequency) == 20){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 1:
            if(value < (x->amplitude) && (x->frequency) == 25){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 2:
            if(value < (x->amplitude) && (x->frequency) == 31){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 3:
            if(value < (x->amplitude) && (x->frequency) == 40){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 4:
            if(value < (x->amplitude) && (x->frequency) == 50){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 5:
            if(value < (x->amplitude) && (x->frequency) == 63){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 6:
            if(value < (x->amplitude) && (x->frequency) == 80){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 7:
            if(value < (x->amplitude) && (x->frequency) == 100){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 8:
            if(value < (x->amplitude) && (x->frequency) == 125){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 9:
            if(value < (x->amplitude) && (x->frequency) == 160){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 10:
            if(value < (x->amplitude) && (x->frequency) == 200){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 11:
            if(value < (x->amplitude) && (x->frequency) == 250){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 12:
            if(value < (x->amplitude) && (x->frequency) == 315){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 13:
            if(value < (x->amplitude) && (x->frequency) == 400){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 14:
            if(value < (x->amplitude) && (x->frequency) == 500){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 15:
            if(value < (x->amplitude) && (x->frequency) == 630){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 16:
            if(value < (x->amplitude) && (x->frequency) == 800){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 17:
            if(value < (x->amplitude) && (x->frequency) == 1000){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 18:
            if(value < (x->amplitude) && (x->frequency) == 1250){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 19:
            if(value < (x->amplitude) && (x->frequency) == 1600){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 20:
            if(value < (x->amplitude) && (x->frequency) == 2000){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 21:
            if(value < (x->amplitude) && (x->frequency) == 2500){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 22:
            if(value < (x->amplitude) && (x->frequency) == 3150){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 23:
            if(value < (x->amplitude) && (x->frequency) == 4000){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 24:
            if(value < (x->amplitude) && (x->frequency) == 5000){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 25:
            if(value < (x->amplitude) && (x->frequency) == 6300){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 26:
            if(value < (x->amplitude) && (x->frequency) == 8000){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 27:
            if(value < (x->amplitude) && (x->frequency) == 10000){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 28:
            if(value < (x->amplitude) && (x->frequency) == 12500){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 29:
            if(value < (x->amplitude) && (x->frequency) == 16000){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        case 30:
            if(value < (x->amplitude) && (x->frequency) == 20000){
                //qDebug() << x->amplitude;
                value = qMax(value,x->amplitude);
                frequency = x->frequency;
            }
            break;
        default:
            break;
        }
    }
    if(frequency != 0)
    {
        qreal refValue = 1000;
        refValue = refValue/(2*sqrt(2));
        value = value/(2*sqrt(2));
        value = 20*(qLn(value/refValue)/qLn(10));
        point.setX(frequency);
        point.setY(value);
        //qDebug() << point;
        m_points.append(point);
        m_count++;
    }
}

void FFTSetting::start()
{
    m_count = 0;
    m_points.clear();
    setData();
}

/***********************************************************
 * class AutoEQTest
 * *********************************************************/
AutoEQTest::AutoEQTest(QObject *parent) : QObject(parent)
{
    m_time = 0;
    m_timer = new QTimer;
    m_timer->setTimerType(Qt::PreciseTimer);
    // Music *music;
    //fftPlayer = music->player;
    // fftPlaylist = music.playlist;
    m_fftSetting = new FFTSetting(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeOut()));
    connect(m_fftSetting, SIGNAL(spectrumComplete(QList<QPointF>)),this,SLOT(calculateEQ(QList<QPointF>)));
    // connect(fftPlayer,SIGNAL(stateChanged(MusicPlayer::State)),this,SLOT(donePlay(MusicPlayer::State)));
}
AutoEQTest::~AutoEQTest()
{
    m_timer->stop();
    delete m_timer;
    delete m_fftSetting;
}

void AutoEQTest::donePlay(MusicPlayer::State newState)
{

    if(newState == MusicPlayer::PausedState)
    {
        //qDebug() << "player pause!";
    }
    if(newState == MusicPlayer::StoppedState)
    {
        //qDebug() << "timer is stopping!!";
        if(m_timer->isActive())
            m_timer->stop();
        if(m_fftSetting->loadFile(m_fileName))
            m_fftSetting->start();
        //测试完成后将测试文件从列表移除
        fftPlaylist->removeMedia(fftPlaylist->getMedia().length()-1);
    }
    if(newState == MusicPlayer::PlayingState)
    {
        //qDebug() << "player play!";
    }
}

void AutoEQTest::timeOut()
{
    m_time++;
    //qDebug()<<"----time is>>>>"<<m_time<<endl;
    if(m_time == BAND_EQ){
        system("killall arecord");
        systemTime = QDateTime::currentDateTime().toMSecsSinceEpoch() - systemTime;
        //qDebug()<<"----systemTime is>>>>"<<systemTime<<endl;
    }
    if(m_time == 60)
        m_timer->stop();
}

void AutoEQTest::autoTest()
{

    QString testFile="/home/root/music/testFile.wav";
    // QString testFile="/home/jia/音乐/testFile.wav";
    //将测试文件添加到播放列表结尾，并播放

    if(fftPlaylist->isEmpty()){

        fftPlayer->setPlaylist(fftPlaylist);
    }
    fftPlaylist->appendMedia(testFile);
    // qDebug()<< "lenght="<<QString::number(fftPlaylist->getMedia().length());
    fftPlaylist->m_currentIndex=fftPlaylist->getMedia().length()-1;



    fftPlayer->play();
    fftPlayer->isTesting=true; //正在测试设置为true
    // system("arecord -r 44100 -f S16_LE /home/jia/公共的/fftaudio.wav &");

    system("arecord -D hw:1,0 -r 44100 -f S16_LE /home/root/music/fftaudio.wav &");
    // ////system("arecord -D hw:1,0 -r 44100 -f S16_LE /home/jia/公共的/fftaudio.wav &");

    m_fileName = "/home/root/music/fftaudio.wav";
    //m_fileName = "/home/jia/公共的/fftaudio.wav";
    //m_fileName = "/home/jia/音乐/testFile.wav";

    m_timer->start(1000);
    systemTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    m_time = 0;
}

//void AutoEQTest::calculateEQ(QList<QPointF> points)
//{
//    m_points = points;
//    if(m_points.size() != BAND_EQ)
//    {
//        qDebug()<<"m_points.size() != BAND_EQ"<<endl;
//        return;
//    }
//    QList<QString> value;
//    qreal tempAbs = 0;
//    qreal tempMax=0;
//    for(int i = 0; i < BAND_EQ; i++)
//    {
//        if(tempMax < m_points.at(i).y())
//            tempMax = m_points.at(i).y();
//    }
//    for(int i = 7; i < 27; i++)//100~8k abs
//        tempAbs = tempAbs + m_points.at(i).y();
//    tempAbs = tempAbs / 20;
//    //qDebug()<<"-----tempAbs="<<tempAbs<<"---------";
//    if(tempAbs < 0 && tempMax < 12){//
//        emit calculateError();
//        return;
//    }
//    for(int i = 0; i < BAND_EQ; i++)
//    {
//        qreal temp = tempAbs - m_points.at(i).y();
//        if(i<5)
//            temp = temp/10;
//        if(i == 30)
//            temp = temp/10;
//        if(temp > 10)
//            temp = 10;
//        if(temp < -10)
//            temp = -10;
//        value.append(QString::number(int(temp)));
////        qDebug() << "ABS = " << m_points.at(i).y();
////        qDebug() << i << " << " << temp;
//    }

//    emit calculateFinish(value);
//}

//void AutoEQTest::calculateEQ(QList<QPointF> points)
//{
//    m_points = points;
//    if(m_points.size() != BAND_EQ)
//    {
//        qDebug()<<"m_points.size() != BAND_EQ"<<endl;
//        return;
//    }
//    QList<QString> value;
//    qreal tempAbs = 0;
//    qreal tempMax = 0;
//    QList<QString> coe;
//    coe << "0.1" << "0.1" << "0.2" << "0.4" << "0.6"
//        << "0.6" << "0.6" << "0.6" << "0.6" << "0.6"
//        << "0.8" << "0.8" << "0.8" ; //315hz

//    for(int i = 0; i < BAND_EQ; i++)
//    {
//        if(tempMax < m_points.at(i).y())
//            tempMax = m_points.at(i).y();
//    }
//    for(int i = 7; i < 27; i++)//100~8k abs
//        tempAbs = tempAbs + m_points.at(i).y();
//    tempAbs = tempAbs / 20;
//    //qDebug()<<"-----tempAbs="<<tempAbs<<"---------";
//    if(tempAbs < 0 && tempMax < 12){//
//        emit calculateError();
//        return;
//    }
//    for(int i = 0; i < BAND_EQ; i++)
//    {
//        qreal temp = tempAbs - m_points.at(i).y();
//        if(i < coe.size())
//            temp = temp*(coe.at(i).toDouble());
//        if(i == 30)
//            temp = temp/10;
//        if(temp > 10)
//            temp = 10;
//        if(temp < -10)
//            temp = -10;
//        value.append(QString::number(int(temp)));
//        qDebug() << "ABS = " << m_points.at(i).y();
//        qDebug() << i << " << " << temp;
//    }

//    emit calculateFinish(value);
//}

void AutoEQTest::calculateEQ(QList<QPointF> points)
{
    m_points = points;
    if(m_points.size() != BAND_EQ)
    {
        qDebug()<<"m_points.size() != BAND_EQ"<<endl;
        return;
    }
    QList<QString> value;
    qreal tempAbs = 0;
    qreal tempMax = 0;
    QList<QString> coe;
    coe << "0.1" << "0.1" << "0.2" << "0.4" << "0.6"
        << "0.6" << "0.6" << "0.6" << "0.6" << "0.6"
        << "0.8" << "0.8" << "0.8" << "0.9" << "0.9" << "0.9" << "0.9";//800hz

    for(int i = 0; i < BAND_EQ; i++)
    {
        if(tempMax < m_points.at(i).y())
            tempMax = m_points.at(i).y();
    }
    for(int i = 7; i < 27; i++)//100~8k abs
        tempAbs = tempAbs + m_points.at(i).y();
    tempAbs = tempAbs / 20;
    //qDebug()<<"-----tempAbs="<<tempAbs<<"---------";
    if(tempAbs < 0 && tempMax < 12){//
        emit calculateError();
        return;
    }
    for(int i = 0; i < BAND_EQ; i++)
    {
        qreal temp = tempAbs - m_points.at(i).y();
        if(i < coe.size())
            temp = temp*(coe.at(i).toDouble());
        if(i == 30)
            temp = temp/10;
        if(temp > 10)
            temp = 10;
        if(temp < -10)
            temp = -10;
        value.append(QString::number(int(temp)));
        //        qDebug() << "ABS = " << m_points.at(i).y();
        //        qDebug() << i << " << " << temp;
    }

    emit calculateFinish(value);
}

QList<QPointF> AutoEQTest::getPoints()
{
    return m_points;
}

void AutoEQTest::outputPath(QString path)
{
    QDir outputDir;
    outputDir.setPath(path);
    // Ensure output directory exists and is empty
    if (outputDir.exists()) {
        const QStringList files = outputDir.entryList(QDir::Files);
        QString file;

        foreach (file, files)
        {

            if(file=="outData.txt")
                outputDir.remove(file);
        }
    } else {
        QDir::current().mkdir(path);
    }

    m_textFile.setFileName(outputDir.filePath("outData.txt"));
    m_textFile.open(QIODevice::WriteOnly | QIODevice::Text);
    m_textStream.setDevice(&m_textFile);
}

bool AutoEQTest::saveEQData(const QString savePath, const QList<QString> dataEQ)
{
    //   if(!m_valueEq.isEmpty())
    {
        outputPath(savePath);
        for(int i = 0; i < BAND_EQ; i++)
        {
            m_textStream << dataEQ.at(i) << "\n";
            m_textStream.flush();
        }
        m_textFile.close();
        system("sync");
        // qDebug()<<"-------------saved-----------"<<endl;
        return true;
    }
    //  else
    //QMessageBox::information(this, "Save EQ data", "no EQ data need be saved!!");
    return false;
}
//加载eq设置文件，如果没有则创建文件，保存eq设置并返回数值列表
QList<QString> AutoEQTest::loadEQData(const QString path)
{
    QFile file;
    QList<QString> values;
    file.setFileName(path);
    // Ensure output directory exists and is empty
    if (file.exists())
    {
        // qDebug()<<"---found output/outData.text!!!!-----"<<endl;
        m_textFile.setFileName(path);
        m_textFile.open(QIODevice::ReadOnly | QIODevice::Text);
        m_textStream.setDevice(&m_textFile);
        for(int i = 0; i < BAND_EQ; i++)
        {
            // qDebug()<<m_textStream.readLine()<<endl;
            values.append(m_textStream.readLine());
        }

        m_textFile.close();

        return values;
    }
    else {

    }
    return values;
}
