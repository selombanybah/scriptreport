# -------------------------------------------------
# Project created by QtCreator 2010-04-08T16:44:34
# -------------------------------------------------
QT += sql \
    script
QT -= gui
TARGET = srsql
TEMPLATE = lib
DESTDIR = ../compiled/script
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
DEFINES += QT_NO_CAST_FROM_ASCII \
    QT_NO_CAST_TO_ASCII

# The application version
VERSION = 0.1

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
