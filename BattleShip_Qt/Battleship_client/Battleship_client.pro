#-------------------------------------------------
#
# Project created by QtCreator 2018-05-03T18:40:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Battleship_client
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    askserver.cpp \
    askname.cpp \
    globals.cpp \
    msghandling.cpp \
    fillgridwindow.cpp \
    qgridsquare.cpp \
    linksquaremap.cpp \
    linkclientsquaremap.cpp \
    waitingwindow.cpp \
    endofgamewindow.cpp

HEADERS  += client.h \
    askserver.h \
    askname.h \
    globals.h \
    fillgridwindow.h \
    qgridsquare.h \
    waitingwindow.h \
    endofgamewindow.h

FORMS    += client.ui \
    askserver.ui \
    askname.ui \
    fillgridwindow.ui \
    waitingwindow.ui \
    endofgamewindow.ui

CONFIG += c++11
