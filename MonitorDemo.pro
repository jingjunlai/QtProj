#-------------------------------------------------
#
# Project created by QtCreator 2019-03-28T10:53:04
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MonitorDemo
TEMPLATE = app


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
    model.cpp \
    bm100amodel.cpp

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
    model.h \
    bm100amodel.h

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
    dialogmoduleinfo.ui
