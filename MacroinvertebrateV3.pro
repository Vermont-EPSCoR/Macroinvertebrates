#-------------------------------------------------
#
# Project created by QtCreator 2016-04-08T12:23:16
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MacroinvertebrateV3
TEMPLATE = app


SOURCES += main.cpp \
    application.cpp \
    views/homeview.cpp \
    views/locationview.cpp \
    views/streamview.cpp \
    views/invertebrateview.cpp \
    views/syncview.cpp \
    models/streamlist.cpp \
    models/stream.cpp \
    models/invertebratelist.cpp \
    models/invertebrate.cpp \
    datamanager.cpp \
    parsers/streamhandler.cpp \
    parsers/invertebratehandler.cpp \
    views/aboutview.cpp \
    views/singlestreamview.cpp \
    webdatasynchronizer.cpp

HEADERS  += \
    application.h \
    views/homeview.h \
    views/locationview.h \
    views/streamview.h \
    views/invertebrateview.h \
    views/syncview.h \
    models/streamlist.h \
    models/stream.h \
    models/invertebratelist.h \
    models/invertebrate.h \
    datamanager.h \
    parsers/streamhandler.h \
    parsers/invertebratehandler.h \
    views/aboutview.h \
    views/singlestreamview.h \
    webdatasynchronizer.h

FORMS    += \
    views/homeview.ui \
    views/locationview.ui \
    views/streamview.ui \
    views/invertebrateview.ui \
    views/syncview.ui \
    views/aboutview.ui \
    views/singlestreamview.ui

CONFIG += c++11
QMAKE_CXXFLAGS += -O3
#QMAKE_CXXFLAGS += -O0 -g
CONFIG += mobility
MOBILITY = 

RESOURCES += \
    media.qrc

