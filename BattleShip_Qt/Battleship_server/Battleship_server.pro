#-------------------------------------------------
#
# Project created by QtCreator 2018-03-11T16:39:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Battleship_server
TEMPLATE = app


SOURCES += main.cpp\
        server.cpp \
    askport.cpp \
    globals.cpp \
    msghandling.cpp \
    qgridsquare.cpp \
    linkserversquaremap.cpp

HEADERS  += server.h \
    askport.h \
    globals.h \
    qgridsquare.h

FORMS    += server.ui \
    askport.ui

CONFIG += c++11
