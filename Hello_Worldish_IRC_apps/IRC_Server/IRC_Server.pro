#-------------------------------------------------
#
# Project created by QtCreator 2017-11-25T21:38:42
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IRC_Server
TEMPLATE = app


SOURCES += main.cpp\
        server.cpp \
    askport.cpp \
    connthread.cpp \
    rwthread.cpp

HEADERS  += server.h \
    askport.h \
    connthread.h \
    rwthread.h

FORMS    += server.ui \
    askport.ui
