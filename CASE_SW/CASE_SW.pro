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
    CDataModelException.cpp \
    CScriptGenerator.cpp \
    CLangWidget.cpp \
    rework/mainwindow.cpp \
    rework/CModelView.cpp \
    rework/CEntityItem.cpp \
    rework/resizedialog.cpp \
    rework/CRelationshipItem.cpp \
    rework/CDataModel.cpp \
    rework/CEntity.cpp \
    rework/CRelationship.cpp \
    rework/CRow.cpp \
    rework/CForeignRow.cpp

HEADERS  += \
    CDataModelException.h \
    CPluginInterface.h \
    CScriptGenerator.h \
    CLangWidget.h \
    rework/mainwindow.h \
    rework/CModelView.h \
    rework/CEntityItem.h \
    rework/resizedialog.h \
    rework/CRelationshipItem.h \
    rework/CDataModel.h \
    rework/CEntity.h \
    rework/CRelationship.h \
    rework/CRow.h \
    rework/CForeignRow.h

FORMS    += \
    rework/mainwindow.ui \
    rework/resizedialog.ui

RESOURCES += \
    images.qrc
