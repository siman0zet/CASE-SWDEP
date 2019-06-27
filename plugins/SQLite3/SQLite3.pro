#-------------------------------------------------
#
# Project created by QtCreator 2015-04-20T16:15:39
#
#-------------------------------------------------

QT       -= gui

TARGET = SQLite3
TEMPLATE = lib
CONFIG += static

INCLUDEPATH += ../../CASE_SW

SOURCES += SQLite3.cpp

HEADERS += SQLite3.h

unix {
    target.path = /usr/lib
    INSTALLS += headers target
    DESTDIR = $$PWD/../../build/lib/
}

win32 {
    CONFIG += dll
    DLLDESTDIR = $$PWD/../../build/lib/
}
