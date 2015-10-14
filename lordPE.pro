#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T07:02:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lordPE
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pefile.cpp

HEADERS  += mainwindow.h \
    pefile.h \
    peinfo.h

FORMS    += mainwindow.ui
