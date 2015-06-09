#-------------------------------------------------
#
# Project created by QtCreator 2015-06-08T23:16:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = But2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    renderthread.cpp \
    buterwidget.cpp

HEADERS  += mainwindow.h \
    renderthread.h \
    buterwidget.h

FORMS    += mainwindow.ui
