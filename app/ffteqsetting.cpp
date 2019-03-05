//#include "ffteqsetting.h"
//#include <utils.h>
//#include <fftw.h>
//#include "qmmp/music.h"
///***********************************************************
// * class FFTSetting
// * *********************************************************/
//FFTSetting::FFTSetting(QObject *parent) : QObject(parent)
//{
//    m_file = 0;
//    m_fft = new fftw(this);
//    connect(m_fft,SIGNAL(fftComplete(FrequencySpectrum&)),this,SLOT(getData(FrequencySpectrum&)));
//}

//FFTSetting::~FFTSetting()
//{
//    m_fft->fftClose();
//}

//bool FFTSetting::loadFile(QString filename)
//{
//    if(isWavFile(filename))
//    {
//        m_file = fopen(filename.toLocal8Bit().data(),"r");
//        readHeader();
//        return true;
//    }
//    return false;
//}

//void FFTSetting::readHeader()
//{
//    struct wavfile_header header;
//    if(fread(&header,1,sizeof(header),m_file) > 0)
//    {
//        //qDebug() << "header.byte_rate " << header.byte_rate;
//        m_length = header.num_channels * header.sample_rate;
//        m_spectrum = m_length;
//    }
//}

//QList<QPointF> FFTSetting::getPoints()
//{
//    return m_points;
//}

//bool FFTSetting::isWavFile(const QString filename)
//{
//    QString type = filename.split(".").last();
//    if(type == "wav")
//        return true;
//    return false;
//}

//void FFTSetting::setData()
//{
//    short data[m_length];
//    fftw_complex in[m_length];
//    while((fread(data,sizeof(short),m_length,m_file)) > 0)
//    {
//        for (int i = 0; i < m_length; i++) {
//            double hannWindow = 0.5 * (1 - cos((2 * M_PI * i) / (m_length - 1)));
//            in[i][REAL] = hannWindow * in[i][REAL];
//        }

//        for(int i = 0; i < m_length; i++)
//        {
//            in[i][REAL] = data[i];
//            in[i][IMAG] = 0;
//        }
//        m_fft->initialize(m_length,in);
//        m_fft->fftStart();
//    }
//    qDebug() << "count = " << m_count;
//    fclose(m_file);
//    emit spectrumComplete(m_points);
//}

//void FFTSetting::getData(FrequencySpectrum &spectrum)
//{
//    m_spectrum = spectrum;
//    FrequencySpectrum::const_iterator x = m_spectrum.begin();
//    const FrequencySpectrum::const_iterator end = m_spectrum.end();
//    qreal value = 0.0;
//    qreal frequency = 0.0;
//    QPointF point;
//#if 0
//    for (; x!=end; ++x)
//    {
//        //value = qMax(value,x->amplitude);
//        if(value < (x->amplitude) && (x->frequency) <= 20000 && (x->frequency) > 10){
//            //qDebug() << x->amplitude;
//            value = qMax(value,x->amplitude);
//            frequency = x->frequency;
//        }
//    }
//    //qDebug() << "count = " << m_count;
//    //m_textStream << "count " << m_count << "\n";
//    if(frequency <= 20000)
//    {
//        value = qLn(value);
//        point.setX(frequency);
//        point.setY(value);
//        m_points.append(point);
//        qDebug() << point;
//        m_textStream << frequency << "\t"
//                     << value << "\t" << "\n";
//    }

//#endif
//    for (; x!=end; ++x)
//    {
//        switch (m_count) {
//        case 0:
//            if(value < (x->amplitude) && (x->frequency) == 20){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 1:
//            if(value < (x->amplitude) && (x->frequency) == 25){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 2:
//            if(value < (x->amplitude) && (x->frequency) == 31){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 3:
//            if(value < (x->amplitude) && (x->frequency) == 40){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 4:
//            if(value < (x->amplitude) && (x->frequency) == 50){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 5:
//            if(value < (x->amplitude) && (x->frequency) == 63){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 6:
//            if(value < (x->amplitude) && (x->frequency) == 80){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 7:
//            if(value < (x->amplitude) && (x->frequency) == 100){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 8:
//            if(value < (x->amplitude) && (x->frequency) == 125){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 9:
//            if(value < (x->amplitude) && (x->frequency) == 160){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 10:
//            if(value < (x->amplitude) && (x->frequency) == 200){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 11:
//            if(value < (x->amplitude) && (x->frequency) == 250){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 12:
//            if(value < (x->amplitude) && (x->frequency) == 315){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 13:
//            if(value < (x->amplitude) && (x->frequency) == 400){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 14:
//            if(value < (x->amplitude) && (x->frequency) == 500){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 15:
//            if(value < (x->amplitude) && (x->frequency) == 630){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 16:
//            if(value < (x->amplitude) && (x->frequency) == 800){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 17:
//            if(value < (x->amplitude) && (x->frequency) == 1000){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 18:
//            if(value < (x->amplitude) && (x->frequency) == 1250){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 19:
//            if(value < (x->amplitude) && (x->frequency) == 1600){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 20:
//            if(value < (x->amplitude) && (x->frequency) == 2000){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 21:
//            if(value < (x->amplitude) && (x->frequency) == 2500){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 22:
//            if(value < (x->amplitude) && (x->frequency) == 3150){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 23:
//            if(value < (x->amplitude) && (x->frequency) == 4000){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 24:
//            if(value < (x->amplitude) && (x->frequency) == 5000){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 25:
//            if(value < (x->amplitude) && (x->frequency) == 6300){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 26:
//            if(value < (x->amplitude) && (x->frequency) == 8000){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 27:
//            if(value < (x->amplitude) && (x->frequency) == 10000){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 28:
//            if(value < (x->amplitude) && (x->frequency) == 12500){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 29:
//            if(value < (x->amplitude) && (x->frequency) == 16000){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        case 30:
//            if(value < (x->amplitude) && (x->frequency) == 20000){
//                //qDebug() << x->amplitude;
//                value = qMax(value,x->amplitude);
//                frequency = x->frequency;
//            }
//            break;
//        default:
//            break;
//        }
//    }
//    if(frequency != 0)
//    {
//        value = qLn(value);
//        point.setX(frequency);
//        point.setY(value);
//        qDebug() << point;
//        m_points.append(point);
//        m_count++;
//    }
//}

//void FFTSetting::start()
//{
//    m_count = 0;
//    m_points.clear();
//    setData();
//}

///***********************************************************
// * class AutoEQTest
// * *********************************************************/
//AutoEQTest::AutoEQTest(QObject *parent) : QObject(parent)
//{
//    m_time = 0;
//    m_timer = new QTimer;
//    m_timer->setTimerType(Qt::PreciseTimer);
//  //   player = new MusicPlayer(this);
//  // playlist = new MusicPlaylist(this);
//    m_fftSetting = new FFTSetting(this);
//   // connect(player,SIGNAL(stateChanged(MusicPlayer::State)),this,SLOT(donePlay(MusicPlayer::State)));
//  //  connect(m_timer,SIGNAL(timeout()),this,SLOT(timeOut()));
//   // connect(m_fftSetting, SIGNAL(spectrumComplete(QList<QPointF>)),this,SLOT(calculateEQ(QList<QPointF>)));
//}

//void AutoEQTest::play()
//{
//    player->play();
//    //system("arecord -D hw:1,0 -r 44100 -f S16_LE /home/root/fftaudio.wav &");
//    system("arecord -r 44100 -f S16_LE /home/public/fftaudio.wav &");
//    //m_fileName = "/home/root/fftaudio.wav";
//    m_fileName = "/home/public/fftaudio.wav";
//    m_timer->start(1000);
//    systemTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
//    qDebug() << "timer is starting!!";
//    m_time = 0;
//}

//void AutoEQTest::donePlay(MusicPlayer::State newState)
//{
//    if(newState == MusicPlayer::PausedState)
//    {
//        qDebug() << "player pause!";
//    }
//    if(newState == MusicPlayer::StoppedState)
//    {
//        qDebug() << "timer is stopping!!";
//        if(m_timer->isActive())
//            m_timer->stop();
//        if(m_fftSetting->loadFile(m_fileName))
//            m_fftSetting->start();
//    }
//    if(newState == MusicPlayer::PlayingState)
//    {
//        qDebug() << "player play!";
//    }
//}

//void AutoEQTest::timeOut()
//{
//    m_time++;
//    if(m_time == BAND_EQ){
//        system("killall arecord");
//        systemTime = QDateTime::currentDateTime().toMSecsSinceEpoch() - systemTime;
//        qDebug() << systemTime;
//    }
//    if(m_time == 60)
//        m_timer->stop();
//    qDebug() << "timer is " + QString::number(m_time, 10);
//}

//void AutoEQTest::autoTest(const QString filePath)
//{
//    playlist->addMedia(filePath);
//    player->setPlaylist(playlist);
//    play();
//}

//void AutoEQTest::calculateEQ(QList<QPointF> points)
//{
//    m_points = points;
//   // qDebug() << "m_points = " << m_points.size() << "BAND_EQ = " << BAND_EQ;
//    Q_ASSERT(m_points.size() == BAND_EQ);
//    QList<QString> value;
//    qreal tempAbs;

//    for(int i = 12; i < 24; i++)//100~4k abs
//        tempAbs = tempAbs + m_points.at(i).y();
//    tempAbs = tempAbs / 12;
//    //if(tempAbs > 9)
//    if(1)
//    {
//        for(int i = 0; i < BAND_EQ; i++)
//        {
//            qreal temp = tempAbs - m_points.at(i).y();
//            if(0.5 < qAbs(temp) && qAbs(temp) < 1)
//                temp = 1;
//            value.append(QString::number(int(temp)));
//            qDebug() << "ABS = " << m_points.at(i).y();
//            qDebug() << i << " << " << temp;
//        }
//    }
//    else {
//        qDebug() << "volume should be louder! please test one more time";
//        //QMessageBox::information(this, "Audio EQ test", "volume should be louder!!");
//    }
//    emit calculateFinish(value);
//}

//QList<QPointF> AutoEQTest::getPoints()
//{
//    return m_points;
//}

//void AutoEQTest::outputPath(QString path)
//{
//    QDir outputDir;
//    outputDir.setPath(path);
//    // Ensure output directory exists and is empty
//    if (outputDir.exists()) {
//        const QStringList files = outputDir.entryList(QDir::Files);
//        QString file;
//        foreach (file, files)
//            outputDir.remove(file);
//    } else {
//        QDir::current().mkdir(path);
//    }
//    m_textFile.setFileName(outputDir.filePath("outData.txt"));
//    m_textFile.open(QIODevice::WriteOnly | QIODevice::Text);
//    m_textStream.setDevice(&m_textFile);
//}

//bool AutoEQTest::saveEQData(const QString savePath, const QList<QString> dataEQ)
//{
// //   if(!m_valueEq.isEmpty())
//    {
//        outputPath(savePath);
//        for(int i = 0; i < BAND_EQ; i++)
//            m_textStream << dataEQ.at(i) << "\n";
//        m_textFile.close();
//        return true;
//    }
//  //  else
//        //QMessageBox::information(this, "Save EQ data", "no EQ data need be saved!!");
//    return false;
//}

//QList<QString> AutoEQTest::loadEQData(const QString path)
//{
//    QFile file;
//    QList<QString> values;
//    file.setFileName(path);
//    // Ensure output directory exists and is empty
//    if (file.exists())
//    {
//         m_textFile.setFileName(path);
//         m_textFile.open(QIODevice::ReadOnly | QIODevice::Text);
//         m_textStream.setDevice(&m_textFile);
//         for(int i = 0; i < BAND_EQ; i++)
//         {
//             values.append(m_textStream.readLine());
//         }
//         m_textFile.close();
//         return values;
//    }
//    else {
//        //QMessageBox::information(this, "load EQ data", "no such data file in this directory!!");
//    }
//    return values;
//}
