#-------------------------------------------------
#
# Project created by QtCreator 2010-08-21T12:10:34
#
#-------------------------------------------------

QT       += network
QT       -= gui
CONFIG += plugin
TARGET = marklin
TEMPLATE = lib
DEFINES += MARKLIN_LIBRARY
DEFINES += QEXTSERIALPORT_USING_SHARED
SOURCES += marklin.cpp
HEADERS += marklin.h \
    traininterface.h
DESTDIR = /home/karasu/trendigital
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /home/karasu/trendigital/qextserialport/src
QMAKE_LIBDIR += /usr/local/lib
QMAKE_LIBDIR += /home/karasu/trendigital/qextserialport/lib
LIBS += -lqextserialport
