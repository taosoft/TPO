#-------------------------------------------------
#
# Project created by QtCreator 2018-05-25T19:25:02
#
#-------------------------------------------------

QT       += core gui charts serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = GUI
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui
