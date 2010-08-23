# -------------------------------------------------
# Project created by QtCreator 2010-04-04T16:27:12
# -------------------------------------------------
QT += script
TARGET = scriptreportengine
TEMPLATE = lib
DESTDIR = ../compiled
DEFINES += SCRIPTREPORTENGINE_LIBRARY
SOURCES += scriptreportengine.cpp \
    sourcetransformer.cpp \
    scriptablepaper.cpp \
    scriptablepapermargins.cpp \
    scriptablepapersize.cpp \
    scriptablereport.cpp \
    textstreamobject.cpp \
    scriptableengine.cpp
HEADERS += scriptreportengine.h \
    scriptreportengine_global.h \
    sourcetransformer.h \
    scriptablepaper.h \
    scriptablepapermargins.h \
    scriptablepapersize.h \
    scriptablereport.h \
    textstreamobject.h \
    scriptableengine.h
DEFINES += QT_NO_CAST_FROM_ASCII \
    QT_NO_CAST_TO_ASCII

# The application version
VERSION = 0.1

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
