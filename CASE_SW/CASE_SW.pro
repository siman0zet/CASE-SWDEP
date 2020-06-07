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
    LIB_PATH=\"\\\"$$PWD/../build/case/lib/\\\"\" \
    LANG_PATH=\"\\\"$$PWD/../build/languages/\\\"\"

SOURCES += \
    dialog/changetabledialog.cpp \
    dialog/cuniquegroupdialog.cpp \
    dialog/resizedialog.cpp \
    editor/crelationshipeditor.cpp \
    editor/ctableeditor.cpp \
    main.cpp \
    main/cpluginwindow.cpp \
    main/mainwindow.cpp \
    main/pmodelwindow.cpp \
    model/cdatamodel.cpp \
    model/cforeignrow.cpp \
    model/cobject.cpp \
    model/crelationship.cpp \
    model/crow.cpp \
    model/ctable.cpp \
    model/cuniquegroup.cpp \
    view/cmodelview.cpp \
    view/cobjectitem.cpp \
    view/crelationshipitem.cpp \
    view/ctableitem.cpp \
    view/pmodelview.cpp

HEADERS  += \
    cplugininterface.h \
    dialog/changetabledialog.h \
    dialog/cuniquegroupdialog.h \
    dialog/resizedialog.h \
    editor/crelationshipeditor.h \
    editor/ctableeditor.h \
    main/cpluginwindow.h \
    main/mainwindow.h \
    main/pmodelwindow.h \
    model/cdatamodel.h \
    model/cforeignrow.h \
    model/cobject.h \
    model/crelationship.h \
    model/crow.h \
    model/ctable.h \
    model/cuniquegroup.h \
    model/idatamodel.h \
    model/iforeingrow.h \
    model/irow.h \
    model/itable.h \
    view/cmodelview.h \
    view/cobjectitem.h \
    view/crelationshipitem.h \
    view/ctableitem.h \
    view/pmodelview.h

FORMS    += \
    dialog/changetabledialog.ui \
    dialog/cuniquegroupdialog.ui \
    dialog/resizedialog.ui \
    editor/crelationshipeditor.ui \
    editor/ctableeditor.ui \
    main/cpluginwindow.ui \
    main/mainwindow.ui \
    main/pmodelwindow.ui

RESOURCES += \
    images.qrc
