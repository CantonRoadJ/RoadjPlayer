#ifndef READCOMMEND_H
#define READCOMMEND_H
#
#include <iostream>
#include <QThread>
#include <QObject>
#include <QString>
#include <QAbstractNativeEventFilter>

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>    //QDBusConnection
#define DBUS_HAL_SERVICE          "org.freedesktop.Hal"
#define DBUS_HAL_PATH              "/org/freedesktop/Hal/Manager"
#define DBUS_HAL_INTERFACE         "org.freedesktop.Hal.Manager"
#define DBUS_HAL_DEVICE_PATH       "/org/freedesktop/Hal/devices"
#define DBUS_HAL_DEVICE_INTERFACE  "org.freedesktop.Hal.Device"

//extern volatile int fd;
class ReadCommand :public QThread{
    Q_OBJECT
    typedef unsigned long ULONG;
public:
    ReadCommand();
    ~ReadCommand();
    Q_INVOKABLE bool openInputDev();
    Q_INVOKABLE void startReading();

signals:
    void nextSongPress();
    void lastSongPress();
    void playSongPress();
    void volumeUpPress();
    void volumeDownPress();
    void keyPress(int keyValue);
    void keyRelease(int keyValue);
    void keyHold(int keyValue);


public :
    void run();
    void readInputCommand();
    void scanInputCommand();

private:
  //  QTimer m_pTimer;

public slots:
    //void readUsbDev();

};
#endif  //readcommand.h
