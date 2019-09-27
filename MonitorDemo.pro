#-------------------------------------------------
#
# Project created by QtCreator 2019-03-28T10:53:04
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += network
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MonitorDemo
TEMPLATE = app
CONFIG += C++11
#DEFINES += QT_NO_DEBUG_OUTPUT

include($$PWD/net/net.pri)
include($$PWD/model/model.pri)
include($$PWD/module/module.pri)
include($$PWD/port/port.pri)
include($$PWD/manager/manager.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    dialogecgset.cpp \
    dialogibpset.cpp \
    dialogtempset.cpp \
    dialogwaveset.cpp \
    dialogtimeset.cpp \
    dialogrespset.cpp \
    dialogspo2set.cpp \
    dialogarrset.cpp \
    dialogparaset.cpp \
    dialogmoduleinfo.cpp \
    mythreadserial.cpp \
    labelecgwave.cpp \
    labelwave.cpp \
    labelspo2wave.cpp \
    labelco2wave.cpp \
    labelibpwave.cpp \
    labelrespwave.cpp \
    labelrespswave.cpp \
    labelnibpwave.cpp \
    idialogset.cpp \
    chartecg.cpp \
    labelecgscale.cpp

HEADERS  += mainwindow.h \
    dialogecgset.h \
    dialogibpset.h \
    dialogtempset.h \
    dialogwaveset.h \
    dialogtimeset.h \
    dialogrespset.h \
    dialogspo2set.h \
    dialogarrset.h \
    dialogparaset.h \
    dialogmoduleinfo.h \
    mythreadserial.h \
    labelecgwave.h \
    qringbuffer_p.h \
    mathtools.h \
    labelwave.h \
    labelspo2wave.h \
    labelco2wave.h \
    labelibpwave.h \
    labelrespwave.h \
    labelrespswave.h \
    labelnibpwave.h \
    idialogset.h \
    chartecg.h \
    labelecgscale.h


FORMS    += mainwindow.ui \
    dialogecgset.ui \
    dialogibpset.ui \
    dialogtempset.ui \
    dialogwaveset.ui \
    dialogtimeset.ui \
    dialogrespset.ui \
    dialogspo2set.ui \
    dialogarrset.ui \
    dialogparaset.ui \
    dialogmoduleinfo.ui \
    dynamicecg.ui

