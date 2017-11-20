#-------------------------------------------------
#
# Project created by QtCreator 2017-11-17T13:39:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EightQueens_threads
TEMPLATE = app


SOURCES += main.cpp\
        queens.cpp \
    queenthread.cpp \
    printqueen.cpp

HEADERS  += queens.h \
    queenthread.h

FORMS    += queens.ui
