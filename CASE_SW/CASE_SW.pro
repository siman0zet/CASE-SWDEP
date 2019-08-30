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
    main/MainWindow.cpp \
    model/CDataModel.cpp \
    model/CForeignRow.cpp \
    model/CObject.cpp \
    model/CRelationship.cpp \
    model/CRow.cpp \
    model/CTable.cpp \
    model/CUniqueGroup.cpp \
    view/CModelView.cpp \
    view/CObjectItem.cpp \
    view/CRelationshipItem.cpp \
    view/CTableItem.cpp \
    editor/CRelationshipEditor.cpp \
    editor/CTableEditor.cpp \
    dialog/ChangeTableDialog.cpp \
    dialog/CUniqueGroupDialog.cpp \
    dialog/ResizeDialog.cpp \
    main/PModelWindow.cpp \
    view/PModelView.cpp \
    main/CPluginWindow.cpp

HEADERS  += \
    CPluginInterface.h \
    main/MainWindow.h \
    model/CDataModel.h \
    model/CForeignRow.h \
    model/CObject.h \
    model/CRelationship.h \
    model/CRow.h \
    model/CTable.h \
    model/CUniqueGroup.h \
    view/CModelView.h \
    view/CObjectItem.h \
    view/CRelationshipItem.h \
    view/CTableItem.h \
    editor/CRelationshipEditor.h \
    editor/CTableEditor.h \
    dialog/ChangeTableDialog.h \
    dialog/CUniqueGroupDialog.h \
    dialog/ResizeDialog.h \
    main/PModelWindow.h \
    view/PModelView.h \
    model/IDataModel.h \
    model/ITable.h \
    model/IRow.h \
    main/CPluginWindow.h \
    model/IForeingRow.h

FORMS    += \
    main/MainWindow.ui \
    editor/CRelationshipEditor.ui \
    editor/CTableEditor.ui \
    dialog/ChangeTableDialog.ui \
    dialog/CUniqueGroupDialog.ui \
    dialog/ResizeDialog.ui \
    main/PModelWindow.ui \
    main/CPluginWindow.ui

RESOURCES += \
    images.qrc
