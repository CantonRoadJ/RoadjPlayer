#Common settings for Qmmp build

#Some conf to redirect intermediate stuff in separate dirs
#UI_DIR=./.build/ui/
#MOC_DIR=./.build/moc/
#OBJECTS_DIR=./.build/obj
#RCC_DIR=./.build/rcc

QT += widgets
DEFINES += QMMP_WS_X11

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050400

#Version

QMMP_VERSION = 1.2.0

#Comment/uncomment this if you want to change plugins list

CONFIG += JACK_PLUGIN
CONFIG += FLAC_PLUGIN
#CONFIG += MUSEPACK_PLUGIN
CONFIG += FFMPEG_PLUGIN
CONFIG += MODPLUG_PLUGIN
#CONFIG += WILDMIDI_PLUGIN
CONFIG += GME_PLUGIN
#CONFIG += XMP_PLUGIN
CONFIG += FFAP_PLUGIN
#CONFIG += OPUS_PLUGIN
CONFIG += OSS_PLUGIN #deprecated
CONFIG += PULSE_AUDIO_PLUGIN
CONFIG += ALSA_PLUGIN
CONFIG += OSS4_PLUGIN
CONFIG += AAC_PLUGIN
#CONFIG += MPLAYER_PLUGIN
CONFIG += CDAUDIO_PLUGIN
CONFIG += BS2B_PLUGIN
CONFIG += SOXR_PLUGIN
CONFIG += LADSPA_PLUGIN
CONFIG += SRC_PLUGIN
CONFIG += UDISKS2_PLUGIN
CONFIG += HAL_PLUGIN
#CONFIG += SID_PLUGIN
CONFIG += QTMULTIMEDIA_PLUGIN

#additional features

CONFIG += WITH_ENCA
CONFIG += WITH_SKINNED
CONFIG += WITH_QSUI

CONFIG -= $$DISABLED_PLUGINS

DISTFILES += \
    $$PWD/qmmp.pri

HEADERS += \
    $$PWD/volumecontrol_p.h \
    $$PWD/volume.h \
    $$PWD/visualfactory.h \
    $$PWD/visualbuffer_p.h \
    $$PWD/visual.h \
    $$PWD/tagreadandwrite.h \
    $$PWD/tagmodel.h \
    $$PWD/statehandler.h \
    $$PWD/spekfactory.h \
    $$PWD/soundcore.h \
    $$PWD/replaygain_p.h \
    $$PWD/recycler_p.h \
    $$PWD/qmmpsettings.h \
    $$PWD/qmmpplugincache_p.h \
    $$PWD/qmmpevents_p.h \
    $$PWD/qmmpaudioengine_p.h \
    $$PWD/qmmp.h \
    $$PWD/outputwriter_p.h \
    $$PWD/outputfactory.h \
    $$PWD/output.h \
    $$PWD/metadatamodel.h \
    $$PWD/metadatamanager.h \
    $$PWD/inputsourcefactory.h \
    $$PWD/inputsource.h \
    $$PWD/fileinputsource_p.h \
    $$PWD/fileinfo.h \
    $$PWD/eqsettings.h \
    $$PWD/enginefactory.h \
    $$PWD/emptyinputsource_p.h \
    $$PWD/effectfactory.h \
    $$PWD/effect.h \
    $$PWD/dithering_p.h \
    $$PWD/decoderfactory.h \
    $$PWD/decoder.h \
    $$PWD/channelmap.h \
    $$PWD/channelconverter_p.h \
    $$PWD/buffer.h \
    $$PWD/audioparameters.h \
    $$PWD/audioconverter.h \
    $$PWD/abstractengine.h

SOURCES +=


