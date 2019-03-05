#include "musicplayer.h"
#include "musicplaylist.h"
#include "qmmp/soundcore.h"
#include <qmath.h>
#include <QDebug>

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject(parent)
{
    m_playlist = 0;
    m_music = 0;
    m_state = StoppedState;
    m_musicEnhanced = EnhancedOff;
    m_music = new SoundCore(this);
    m_posOnCircle = 0;
    connect(&m_timer, SIGNAL(timeout()), SLOT(setTimeOut()));
}

MusicPlayer::~MusicPlayer()
{
    m_timer.stop();
    delete m_music;

}

MusicPlayer::State MusicPlayer::state() const
{
    return m_state;
}

MusicPlaylist *MusicPlayer::playlist() const
{
    return m_playlist;
}

qint64 MusicPlayer::duration() const
{
    return m_music->totalTime();
}

qint64 MusicPlayer::position() const
{
    return m_music->elapsed();
}

int MusicPlayer::volume() const
{
    return m_music->volume();
}

bool MusicPlayer::isMuted() const
{
    return m_music->isMuted();
}
EqSettings MusicPlayer::eqSettings() const
{
    return m_music->eqSettings();
}

void MusicPlayer::setEqSettings(const EqSettings &settings)
{
    m_music->setEqSettings(settings);
}

void MusicPlayer::setMusicEnhanced(Enhanced type)
{
    m_musicEnhanced = type;
}

MusicPlayer::Enhanced MusicPlayer::getMusicEnhanced() const
{
    return m_musicEnhanced;
}

QStringList MusicPlayer::supportFormatsString()
{
    return QStringList()<< "mp3" << "mp2" << "mp1" << "wav" << "ogg"
                        << "flac" << "ac3" << "aac" << "oga" << "pcm"
                        << "ape" << "wma"<<"m4a";
}

QStringList MusicPlayer::supportFormatsFilterString()
{
    return QStringList()<< "*.mp3" << "*.mp2" << "*.mp1" << "*.wav"
                        << "*.ogg" << "*.flac" << "*.ac3" << "*.aac"
                        << "*.oga" << "*.pcm" << "*.ape"<< "*.wma"<<"*.m4a";
}

QStringList MusicPlayer::supportFormatsFilterDialogString()
{
    return QStringList()<< "Mp3 File(*.mp3)" << "Mp2 File(*.mp2)" << "Mp1 File(*.mp1)"
                        << "Wav File(*.wav)" << "Ogg File(*.ogg)" << "Flac File(*.flac)"
                        << "Ac3 File(*.ac3)" << "Aac File(*.aac)" << "Oga File(*.oga)"
                        << "Pcm File(*.pcm)" << "Ape File(*.ape)" << "Wma File(*.wma)";
}

#ifdef Q_OS_WIN
void MusicPlayer::setSpectrum(HWND wnd, int w, int h, int x, int y)
{
    //    if(!m_music)
    //    {
    //        return;
    //    }
    //    /// set graph type to AREA, left channel on top
    //    m_music->SetFFTGraphParam(gpGraphType, gtAreaLeftOnTop);
    //    /// set linear scale
    //    m_music->SetFFTGraphParam(gpHorizontalScale, gsLinear);
    //    m_music->DrawFFTGraphOnHWND(wnd, x, y, w, h);
}
#endif

void MusicPlayer::play()
{ 
    if(m_playlist->isEmpty()){
        m_state = StoppedState;
        return;
    }
    m_state=PlayingState;

    Qmmp::State state = m_music->state();
    if(m_currentMedia==m_playlist->currentMediaString()&&state==Qmmp::Paused){
        m_music->pause(); //pause or resume
        m_timer.start(1000);
        return;
    }
    m_currentMedia = m_playlist->currentMediaString();
    if(!m_music->play(m_currentMedia))
    {
        m_state = StoppedState;
        return;
    }
    m_timer.start(1000);
    emit positionChanged(0);
    emit durationChanged(m_music->totalTime());
}
void MusicPlayer::playByName(QString Path){

    //    m_state = PlayingState;


    //    emit stateChanged(m_state);
    //    //m_currentMedia = m_playlist->currentMediaString();
    //    ///The current playback path
    //    if(!m_music->play(Path))
    //    {
    //        m_state = StoppedState;
    //        //        M_LOGGER << "Error1." << m_music->GetError() << LOG_END;
    //        qDebug()<<"-------m_state = StoppedState!!!!!!!"<<endl;
    //        return;
    //    }
    //    m_timer.start(1000);
}
void MusicPlayer::playNext()
{
    int index = m_playlist->currentIndex();
    m_playlist->setCurrentIndex((++index >= m_playlist->mediaCount()) ? 0 : index);
}

void MusicPlayer::playPrivious()
{
    int index = m_playlist->currentIndex();
    m_playlist->setCurrentIndex((--index < 0) ? 0 : index );
}

void MusicPlayer::pause()
{
    m_music->pause();
    m_timer.stop();
    m_state = PausedState;
}

void MusicPlayer::stop()
{
    m_music->stop();
    m_timer.stop();
    m_state = StoppedState;
}

void MusicPlayer::setPosition(qint64 position)
{
    m_music->seek(position);
}

void MusicPlayer::setVolume(int volume)
{
    m_music->setVolume(volume);
}

void MusicPlayer::setVolume(int left, int right)
{
    m_music->setVolume(left,right);
}

void MusicPlayer::setMuted(bool muted)
{
    m_music->setMuted(muted);
}

void MusicPlayer::setPlaylist(MusicPlaylist *playlist)
{
    m_playlist = playlist;
    connect(m_playlist, SIGNAL(removeCurrentMedia()), SLOT(removeCurrentMedia()));
}

void MusicPlayer::setTimeOut()
{

    if(!isTesting)
    {
        emit positionChanged( position() );
    }


    Qmmp::State state = m_music->state();
    //歌曲切换
    if(state != Qmmp::Playing && state != Qmmp::Paused)
    {
        m_timer.stop();
        if(isTesting){ //测试结束>停止
            m_music->stop();
            emit stateChanged(StoppedState);
            emit testOver();
            return;
        }else{
            if(m_playlist->playbackMode() == MC_PlayOnce)
            {
                m_music->stop();
                emit positionChanged(0);
                emit stateChanged(StoppedState);
                return;
            }
            m_playlist->setCurrentIndex();
            if(m_playlist->playbackMode() == MC_PlayOrder &&
                    m_playlist->currentIndex() == -1)
            {
                m_music->stop();
                emit positionChanged(0);
                emit stateChanged(StoppedState);
                return;
            }
        }

        play();

        //        case MC_PlayOneLoop: break;
        //        case MC_PlayOrder:
        //            if(++m_currentIndex >= m_mediaList.count())
        //            {
        //                m_currentIndex = -1;
        //            }
        //            break;
        //        case MC_PlayListLoop:
        //            if(++m_currentIndex >= m_mediaList.count())
        //            {
        //                m_currentIndex = 0;
        //            }
        //            break;
        //        case MC_PlayRandom:
        //            m_currentIndex = rand() % m_mediaList.count();
        //            break;
        //        case MC_PlayOnce :
        //            break;
        //        }
    }
}

void MusicPlayer::setMusicEnhancedCase()
{
    //    switch(m_musicEnhanced)
    //    {
    //        case MusicVocal:
    //            setEqEffect(MIntList()<<  0<<  4<<  1<< -5<< -1<<  2<< -2<< -4<< -4<<  0);
    //            break;
    //        case MusicNICAM:
    //            setEqEffect(MIntList()<<-12<<-12<< -9<< -6<< -3<<-12<< -9<< -6<< -3<<-12);
    //            break;
    //        case MusicSubwoofer:
    //            setEqEffect(MIntList()<<  6<<-10<<-10<<  0<<  0<< -3<< -5<< -7<< -9<<-11);
    //            break;
    //        default:
    //            break;
    //    }
}

void MusicPlayer::removeCurrentMedia()
{
    if(m_music)
    {
        m_timer.stop();
        m_music->stop();
    }
}

//void MusicPlayer::setEqEffect(const MIntList &hz)
//{
//    m_equalizer->setEqEffect(hz);
//}

//void MusicPlayer::setEnaleEffect(bool enable)
//{
//    m_equalizer->setEnaleEffect(enable);
//}

//void MusicPlayer::setEqInformation()
//{
//    m_equalizer->readEqInformation();
//}

