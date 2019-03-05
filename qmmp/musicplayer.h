#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTimer>
#include "qmmp/eqsettings.h"

#ifdef Q_OS_WIN
#   include <Windows.h>
#   ifdef Q_CC_GNU
#     pragma GCC diagnostic ignored "-Wswitch"
#   endif
#endif

class SoundCore;
class MusicPlaylist;

class MusicPlayer : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        StoppedState,
        PlayingState,
        PausedState
    };

    enum Enhanced
    {
        EnhancedOff,
        Music3D,
        MusicVocal,
        MusicNICAM,
        MusicSubwoofer
    };

    explicit MusicPlayer(QObject *parent = 0);
    ~MusicPlayer();

    State state() const;
    void setPlaylist(MusicPlaylist *playlist);
    void getPlaylist();
    MusicPlaylist *playlist() const;
    State m_state;
    qint64 duration() const;
    qint64 position() const;
    bool isTesting=false; //是否正在测试
    void playNext();
    void playPrivious();

    int volume() const;
    void setVolume(int volume);
    void setVolume(int left, int right);
    bool isMuted() const;
    void setMuted(bool muted);
    void setPosition(qint64 position);

    EqSettings eqSettings() const;
    void setEqSettings(const EqSettings &settings);

    void setMusicEnhanced(Enhanced type);
    Enhanced getMusicEnhanced() const;

    static QStringList supportFormatsString();
    static QStringList supportFormatsFilterString();
    static QStringList supportFormatsFilterDialogString();

signals:
    void stateChanged(MusicPlayer::State newState);

    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void testOver();
    void volumeChanged(int volume);
    void mutedChanged(bool muted);

public Q_SLOTS:
    void play();
    void pause();
    void stop();
    void playByName(QString Path);

    //    void setEqEffect(const MIntList &hz);
    //    void setEnaleEffect(bool enable);
    //    void setEqInformation();
#ifdef Q_OS_WIN
    void setSpectrum(HWND,int w,int h,int x = 0,int y = 0);
#endif

private Q_SLOTS:
    void setTimeOut();
    void removeCurrentMedia();

protected:
    void setMusicEnhancedCase();

    MusicPlaylist *m_playlist;

    SoundCore *m_music;
    QTimer m_timer;
    QString m_currentMedia;
    Enhanced m_musicEnhanced;

    float m_posOnCircle;


};

#endif // MUSICPLAYER_H
