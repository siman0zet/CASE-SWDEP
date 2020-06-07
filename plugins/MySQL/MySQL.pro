#-------------------------------------------------
#
# Project created by QtCreator 2019-05-04T12:24:07
#
#-------------------------------------------------

QT       -= gui

TARGET = MySQL
TEMPLATE = lib

INCLUDEPATH += ../../CASE_SW

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += mysql.cpp

HEADERS += mysql.h

unix {
    CONFIG += plugin
    DESTDIR = $$PWD/../../build/case/lib/
}

win32 {
    CONFIG += dll
    DLLDESTDIR = $$PWD/../../build/case/lib/
}
