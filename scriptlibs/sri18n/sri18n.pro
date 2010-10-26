include(../../scriptreport.pri)
QT += script
QT -= gui
TARGET = sri18n
TEMPLATE = lib
DESTDIR = ../../compiled/script
CONFIG += plugin
HEADERS += scriptreporti18n.h \
    scriptabletranslator.h \
    scriptablei18n.h \
    scriptablelocale.h
SOURCES += scriptreporti18n.cpp \
    scriptabletranslator.cpp \
    scriptablei18n.cpp \
    scriptablelocale.cpp
