#-------------------------------------------------
#
# Project created by QtCreator 2015-04-20T16:15:39
#
#-------------------------------------------------

QT       -= gui

TARGET = SQLite3
TEMPLATE = lib

INCLUDEPATH += ../../CASE_SW

SOURCES += \
    sqlite3.cpp

HEADERS += \
    sqlite3.h

unix {
    CONFIG += plugin
    DESTDIR = $$PWD/../../build/lib/
}

win32 {
    CONFIG += dll
    DLLDESTDIR = $$PWD/../../build/lib/
}
