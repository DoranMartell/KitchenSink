#-------------------------------------------------
#
# Project created by QtCreator 2017-11-08T18:42:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KnightTourQt
TEMPLATE = app


SOURCES += main.cpp\
        knighttour.cpp \
    moverandom.cpp \
    moveaccessability.cpp \
    colorschanging.cpp \
    printacessability.cpp \
    movebacktrack.cpp

HEADERS  += knighttour.h

FORMS    += knighttour.ui
