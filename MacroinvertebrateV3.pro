#-------------------------------------------------
#
# Project created by QtCreator 2016-04-08T12:23:16
#
#-------------------------------------------------

QT       += core gui network xml svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MacroinvertebrateV3
TEMPLATE = app


SOURCES += application.cpp \
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
    main.cpp \
    models/invertebrate.cpp \
    models/stream.cpp \
    parsers/invertebratehandler.cpp \
    parsers/streamhandler.cpp \
    views/aboutview.cpp \
    views/homeview.cpp \
    views/invertebrateview.cpp \
    views/settingsview.cpp \
    views/singlestreamview.cpp \
    views/streamview.cpp \
    webdatasynchronizer.cpp \
    views/mainwindow.cpp


HEADERS  += application.h \
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
    deps/gumbo-parser/tokenizer.h \
    deps/gumbo-parser/tokenizer_states.h \
    deps/gumbo-parser/utf8.h \
    deps/gumbo-parser/util.h \
    deps/gumbo-parser/vector.h \
    deps/QGumboParser/HtmlTag.h \
    deps/QGumboParser/qgumboattribute.h \
    deps/QGumboParser/qgumbodocument.h \
    deps/QGumboParser/qgumbonode.h \
    models/invertebrate.h \
    models/stream.h \
    parsers/invertebratehandler.h \
    parsers/streamhandler.h \
    views/aboutview.h \
    views/homeview.h \
    views/invertebrateview.h \
    views/settingsview.h \
    views/singlestreamview.h \
    views/streamview.h \
    webdatasynchronizer.h \
    views/mainwindow.h

FORMS    += \
    views/aboutview.ui \
    views/homeview.ui \
    views/invertebrateview.ui \
    views/settingsview.ui \
    views/singlestreamview.ui \
    views/streamview.ui

INCLUDEPATH += "$$PWD/deps/gumbo-parser"
INCLUDEPATH += "$$PWD/deps/QGumboParser"

QMAKE_LFLAGS += -v

CONFIG += c++11
QMAKE_CXXFLAGS += -O3
QMAKE_CFLAGS += -std=c99 -O3  # For Gumbo
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
    media/vbig.icns \
    Info.plist

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

ICON = media/OSX.icns

ios {
    QMAKE_INFO_PLIST = Info.plist
    ios_icon.files = $$files($$PWD/iOS/iPhone*.png)
    QMAKE_BUNDLE_DATA += ios_icon
    DEFINES += "ON_IOS"
}

macx {
# For reloading of the stylesheet during development
    message("Adding file system watcher")
    DEFINES += "ADD_FS_WATCHER"
}

!macx: {
#    These settings will remove qDebug statements and other statements that take too long for mobile processors
    message("Using settings for Mobile.")
    DEFINES += "MOBILE_DEPLOYMENT"
}

