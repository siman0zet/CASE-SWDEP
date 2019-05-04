#-------------------------------------------------
#
# Project created by QtCreator 2015-06-20T00:20:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CASE_SW
TEMPLATE = app

DESTDIR = $$PWD/../build/case/
DEFINES += \
    LIB_PATH=\"\\\"$$PWD/../build/lib/\\\"\" \
    LANG_PATH=\"\\\"$$PWD/../build/languages/\\\"\"

SOURCES += \
    main.cpp \
    CDataModel.cpp \
    CRow.cpp \
    CEntity.cpp \
    CConnection.cpp \
    CForeignRow.cpp \
    CDataModelException.cpp \
    CViewModel.cpp \
    CConnectionView.cpp \
    CEntityView.cpp \
    CDetailedEntityView.cpp \
    CDetailedEntityWidget.cpp \
    CConnectionViewWidget.cpp \
    CLoadAndSaveWidget.cpp \
    CScriptGenerator.cpp \
    CLangWidget.cpp \
    CScrollArea.cpp \
    CUniquesEditorWidget.cpp \
    mainwindow.cpp

HEADERS  += \
    CDataModel.h \
    CRow.h \
    CEntity.h \
    CConnection.h \
    CForeignRow.h \
    CDataModelException.h \
    CViewModel.h \
    CConnectionView.h \
    CEntityView.h \
    CDetailedEntityView.h \
    CDetailedEntityWidget.h \
    CConnectionViewWidget.h \
    CLoadAndSaveWidget.h \
    CPluginInterface.h \
    CScriptGenerator.h \
    CLangWidget.h \
    CScrollArea.h \
    CUniquesEditorWidget.h \
    mainwindow.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    images.qrc
