#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T09:00:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = grammar
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    analyesdialog.cpp \
    lr0.cpp

HEADERS  += mainwindow.h \
    analyesdialog.h \
    lr0.h

FORMS    += mainwindow.ui \
    analyesdialog.ui

DISTFILES +=

RC_FILE  += myico.rc
