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
    models/stream.cpp \
    models/invertebrate.cpp \
    parsers/streamhandler.cpp \
    parsers/invertebratehandler.cpp \
    views/aboutview.cpp \
    views/singlestreamview.cpp \
    webdatasynchronizer.cpp \
    deps/gumbo-parser/attribute.c \
    deps/gumbo-parser/char_ref.c \
    deps/gumbo-parser/error.c \
    deps/gumbo-parser/parser.c \
    deps/gumbo-parser/string_buffer.c \
    deps/gumbo-parser/string_piece.c \
    deps/gumbo-parser/tag.c \
    deps/gumbo-parser/tokenizer.c \
    deps/gumbo-parser/utf8.c \
    deps/gumbo-parser/util.c \
    deps/gumbo-parser/vector.c \
    deps/QGumboParser/qgumboattribute.cpp \
    deps/QGumboParser/qgumbodocument.cpp \
    deps/QGumboParser/qgumbonode.cpp \
    views/settingsview.cpp

HEADERS  += \
    application.h \
    views/homeview.h \
    views/locationview.h \
    views/streamview.h \
    views/invertebrateview.h \
    models/stream.h \
    models/invertebrate.h \
    parsers/streamhandler.h \
    parsers/invertebratehandler.h \
    views/aboutview.h \
    views/singlestreamview.h \
    webdatasynchronizer.h \
    deps/gumbo-parser/attribute.h \
    deps/gumbo-parser/char_ref.h \
    deps/gumbo-parser/char_ref.rl \
    deps/gumbo-parser/error.h \
    deps/gumbo-parser/gumbo.h \
    deps/gumbo-parser/insertion_mode.h \
    deps/gumbo-parser/parser.h \
    deps/gumbo-parser/string_buffer.h \
    deps/gumbo-parser/string_piece.h \
    deps/gumbo-parser/token_type.h \
    deps/gumbo-parser/tokenizer_states.h \
    deps/gumbo-parser/tokenizer.h \
    deps/gumbo-parser/utf8.h \
    deps/gumbo-parser/util.h \
    deps/gumbo-parser/vector.h \
    deps/QGumboParser/HtmlTag.h \
    deps/QGumboParser/qgumboattribute.h \
    deps/QGumboParser/qgumbodocument.h \
    deps/QGumboParser/qgumbonode.h \
    views/settingsview.h

FORMS    += \
    views/homeview.ui \
    views/locationview.ui \
    views/streamview.ui \
    views/invertebrateview.ui \
    views/aboutview.ui \
    views/singlestreamview.ui \
    views/settingsview.ui

INCLUDEPATH += "$$PWD/deps/gumbo-parser"
INCLUDEPATH += "$$PWD/deps/QGumboParser"

#QMAKE_LFLAGS += -v

CONFIG += c++11
QMAKE_CXXFLAGS += -O3
QMAKE_CFLAGS += -std=c99  # For Gumbo
CONFIG += mobility
MOBILITY = 

RESOURCES += \
    media.qrc

DISTFILES += \
    deps/gumbo-parser/tag.in \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    media/vbig.icns

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

ICON = media/OSX.icns

