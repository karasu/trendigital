# -------------------------------------------------
# Project created by QtCreator 2010-07-25T22:09:29
# -------------------------------------------------
QT += core \
    gui \
    sql \
    network \
    opengl

TARGET = trendigital
TEMPLATE = app
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/document.cpp \
    src/dialogs/editlokos.cpp \
    src/dialogs/lokotabbar.cpp \
    src/dialogs/lokodock.cpp \
    src/loko.cpp \
    src/database.cpp \
    src/dialogs/iconview.cpp \
    src/cell.cpp \
    src/element.cpp \
    src/program.cpp \
    src/route.cpp \
    src/lokocommands.cpp \
    src/conditions.cpp \
    src/view.cpp \
    src/plugindialog.cpp \
    src/traininterface.cpp \
    src/debug.cpp
HEADERS += src/mainwindow.h \
    src/document.h \
    src/dialogs/editlokos.h \
    src/general.h \
    src/dialogs/lokotabbar.h \
    src/dialogs/lokodock.h \
    src/loko.h \
    src/database.h \
    src/dialogs/iconview.h \
    src/cell.h \
    src/element.h \
    src/program.h \
    src/route.h \
    src/lokocommands.h \
    src/conditions.h \
    src/view.h \
    src/plugindialog.h \
    src/traininterface.h \
    src/debug.h
FORMS += src/mainwindow.ui \
    src/dialogs/editlokos.ui \
    src/dialogs/lokotabbar.ui \
    src/dialogs/lokodock.ui \
    src/dialogs/iconview.ui
OTHER_FILES += 
RESOURCES += icons.qrc \
    bitmaps.qrc

LIBS += -lz
