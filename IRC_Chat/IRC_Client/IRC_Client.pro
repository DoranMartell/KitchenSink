#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T23:53:08
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IRC_Client
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    servcredentials.cpp \
    loginwindow.cpp \
    receivingthread.cpp \
    regnewwindow.cpp

HEADERS  += client.h \
    servcredentials.h \
    loginwindow.h \
    receivingthread.h \
    regnewwindow.h

FORMS    += client.ui \
    servcredentials.ui \
    loginwindow.ui \
    regnewwindow.ui
