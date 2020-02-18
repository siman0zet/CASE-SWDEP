#-------------------------------------------------
#
# Project created by QtCreator 2015-04-20T16:15:39
#
#-------------------------------------------------

QT       -= gui

TARGET = Postgresql
TEMPLATE = lib

INCLUDEPATH += ../../CASE_SW

SOURCES += postgresql.cpp

HEADERS += postgresql.h

unix {
    CONFIG += plugin
    DESTDIR = $$PWD/../../build/lib/
}

win32 {
    CONFIG += dll
    DLLDESTDIR = $$PWD/../../build/lib/
}
