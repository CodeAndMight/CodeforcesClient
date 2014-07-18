#-------------------------------------------------
#
# Project created by QtCreator 2014-07-18T13:32:51
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CodeforcesClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    codeforcesapi.cpp

HEADERS  += mainwindow.h \
    codeforcesapi.h

FORMS    += mainwindow.ui
