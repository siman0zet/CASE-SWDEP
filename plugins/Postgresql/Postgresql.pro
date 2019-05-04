#-------------------------------------------------
#
# Project created by QtCreator 2015-04-20T16:15:39
#
#-------------------------------------------------

QT       -= gui

TARGET = Postgresql
TEMPLATE = lib

DLLDESTDIR = $$PWD/../../build/lib/
INCLUDEPATH += ../../CASE_SW/

SOURCES += postgresql.cpp

HEADERS += postgresql.h

unix {
    target.path = /usr/lib
    headers.files = include/*
    headers.path = /usr/include
    INSTALLS += headers target
}

win32 {
    CONFIG += dll static
}
