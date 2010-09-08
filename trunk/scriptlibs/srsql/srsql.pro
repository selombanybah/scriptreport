# -------------------------------------------------
# Project created by QtCreator 2010-04-08T16:44:34
# -------------------------------------------------
include(../../scriptreport.pri)
QT += sql \
    script
QT -= gui
TARGET = srsql
TEMPLATE = lib
DESTDIR = ../../compiled/script
CONFIG += plugin
SOURCES += scriptreportsql.cpp \
    scriptablesql.cpp \
    scriptabledatabase.cpp \
    scriptableerror.cpp \
    scriptablequery.cpp \
    scriptablerecord.cpp
HEADERS += scriptreportsql.h \
    scriptablesql.h \
    scriptabledatabase.h \
    scriptableerror.h \
    scriptablequery.h \
    scriptablerecord.h

