#ifndef FFTW_H
#define FFTW_H

#include <QObject>
#include <fft/include/fftw3.h>
#include <fft/include/frequencyspectrum.h>

class fftw : public QObject
{
    Q_OBJECT
public:
    explicit fftw(QObject *parent );
    void fftClose();

private:
    void fftPlan();
    void fftExe();
    void fftCalculate();

signals:
    void fftComplete(FrequencySpectrum &spectrum);

public slots:
    void fftStart();
    void initialize(qint64 length, fftw_complex *in);

private:
    fftw_complex *m_in;
    fftw_complex *m_out;
    fftw_plan m_plan;
    FrequencySpectrum m_spectrum;
    qint64 m_length;
};

#endif // FFTW_H
