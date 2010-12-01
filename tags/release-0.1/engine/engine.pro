include(../scriptreport.pri)
include(scriptable/scriptable.pri)
QT += script
TARGET = scriptreportengine
TEMPLATE = lib
DESTDIR = ../compiled
DEFINES += SCRIPTREPORTENGINE_LIBRARY
SOURCES += scriptreport.cpp \
    scriptreportengine.cpp \
    sourcetransformer.cpp \
    textstreamobject.cpp \
    shell.cpp
HEADERS += scriptreport.h \
    scriptreportengine.h \
    scriptreportengine_global.h \
    sourcetransformer.h \
    textstreamobject.h \
    shell.h

