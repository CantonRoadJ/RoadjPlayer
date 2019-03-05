#-------------------------------------------------
#
# Project created by QtCreator 2018-10-26T10:07:36
#
#-------------------------------------------------

QT += core gui
#virtualkeyboard
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RoadjPlayer
TEMPLATE = app
VERSION += $$Version

#QMAKE_TARGET_PRODUCT = RoadjPlayer1.0
#QMAKE_TARGET_COMPANY = RoadJ
#QMAKE_TARGET_DESCRIPTION = 文件描述
#QMAKE_TARGET_COPYRIGHT = 版权

INCLUDEPATH += ../qmmp/include/
INCLUDEPATH += ../app/
INCLUDEPATH += ../fft/include/

#LIBS += -L$$PWD/lib -lqmmp

##arm :arm-lib
LIBS += -L$$PWD/fft/arm-lib -lfftChart
LIBS += -L$$PWD/fft/arm-lib -lfftw3
LIBS += -L$$PWD/qmmp/arm-lib -lqmmp

#desktop : lib
#LIBS += -L$$PWD/fft/lib -lfftChart
#LIBS += -L$$PWD/fft/lib -lfftw3
#LIBS += -L$$PWD/qmmp/lib -lqmmp
include(version/version.pri)
include(qmmp/include/qmmp.pri)
include(app/app.pri)
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    settingspage.cpp \
    listitem.cpp \
    wifipage.cpp \
    eqsettingspage.cpp \
    wifiitem.cpp \
    mylistwidget.cpp \
    additem.cpp \
    keypad.cpp \ 
    myhslider.cpp

HEADERS += \
        mainwindow.h \
    settingspage.h \
    listitem.h \
    wifipage.h \
    eqsettingspage.h \
    wifiitem.h \
    mylistwidget.h \
    additem.h \
    keypad.h \
    myhslider.h

FORMS += \
        mainwindow.ui \
    settingspage.ui \
    listitem.ui \
    myhslider.ui \
    wifipage.ui \
    eqsettingspage.ui \
    wifiitem.ui \
    keypad.ui

DISTFILES += \
    app/app.pri \

RESOURCES += \
    img.qrc \
    qss.qrc

