# -------------------------------------------------
# Project created by QtCreator 2010-04-04T16:27:12
# -------------------------------------------------
include(../scriptreport.pri)
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
    scriptableengine.cpp \
    scriptableshellengine.cpp \
    shell.cpp \
    scriptreport.cpp
HEADERS += scriptreportengine.h \
    scriptreportengine_global.h \
    sourcetransformer.h \
    scriptablepaper.h \
    scriptablepapermargins.h \
    scriptablepapersize.h \
    scriptablereport.h \
    textstreamobject.h \
    scriptableengine.h \
    scriptableshellengine.h \
    shell.h \
    scriptreport.h
