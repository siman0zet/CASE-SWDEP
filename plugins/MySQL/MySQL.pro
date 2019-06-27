#-------------------------------------------------
#
# Project created by QtCreator 2019-05-04T12:24:07
#
#-------------------------------------------------

QT       -= gui

TARGET = MySQL
TEMPLATE = lib
CONFIG += static

INCLUDEPATH += ../../CASE_SW

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += mysql.cpp

HEADERS += mysql.h

unix {
    target.path = /usr/lib
    INSTALLS += target
    DESTDIR = $$PWD/../../build/lib/
}

win32 {
    CONFIG += dll
    DLLDESTDIR = $$PWD/../../build/lib/
}
