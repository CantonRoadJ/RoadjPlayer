#ifndef FFTEQSETTING_H
#define FFTEQSETTING_H

#include <QObject>
#include <stdio.h>
#include <stdlib.h>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <fft/include/view.h>
#include <fft/include/fftw.h>
#include "musicplayer.h"
#include "musicplaylist.h"
#include "qmmp/include/eqsettings.h"

#define BAND_EQ EqSettings::EQ_BANDS_31

class FFTSetting : public QObject
{
    Q_OBJECT
public:
    explicit FFTSetting(QObject *parent = 0);
    ~FFTSetting();
    /********************************************
    * load audio file
    ***********************************************/
    bool loadFile(QString filename);
    /********************************************
    * read audio file's spectrum data
    ***********************************************/
    void start();

    QList<QPointF> getPoints();

private:
    /********************************************
    * test audio file should be .wav format
    ***********************************************/
    bool isWavFile(const QString filename);
    /********************************************
    * read audio file header data firstly
    ***********************************************/
    void readHeader();
    /********************************************
    * send audio file data to FFT
    ***********************************************/
    void setData();

signals:
    void spectrumComplete(QList<QPointF> points);

public slots:
    /********************************************
    * get the FFT result
    ***********************************************/
    void getData(FrequencySpectrum &spectrum);

private:
    FILE * m_file;
    FrequencySpectrum m_spectrum;
    fftw *m_fft;
    qint64 m_length;
    QList<QPointF> m_points;
    int m_count;
};

class AutoEQTest : public QObject
{
    Q_OBJECT
public:
    explicit AutoEQTest(QObject *parent = 0);
    ~AutoEQTest();
    /********************************************
    * auto play test audio and get the EQ value
    ***********************************************/
    void autoTest();
    /********************************************
    * save the EQ value to the save path outData.txt
    ***********************************************/
    bool saveEQData(const QString savePath, const QList<QString> dataEQ);
    /********************************************
    * load the EQ value from the save path outData.txt
    ***********************************************/
    QList<QString> loadEQData(const QString path);
    /********************************************
    * get the spectrum data
    ***********************************************/
    QList<QPointF> getPoints();
    /********************************************

    * play status changed
    ***********************************************/
    void donePlay(MusicPlayer::State newState);

private:

    /********************************************
    * EQ data output path
    ***********************************************/
    void outputPath(QString path);

signals:
    /********************************************
    * calculate EQ data finished and return
    ***********************************************/
    void calculateFinish(QList<QString> values);
    void calculateError();

private slots:
    /********************************************
    * timer for 1s
    ***********************************************/
    void timeOut();

    /********************************************
    * calculate EQ value
    ***********************************************/
    void calculateEQ(QList<QPointF> points);
public:
    MusicPlayer *fftPlayer;
    MusicPlaylist *fftPlaylist;


private:
    QTimer *m_timer;
    int m_time;
    int m_count;
    int systemTime;
    QList<QPointF> m_points;
    QTextStream m_textStream;
    QFile m_textFile;

    QString m_fileName;
    FFTSetting *m_fftSetting;
};

#endif // FFTEQSETTING_H
