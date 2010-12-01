include(../../scriptreport.pri)
HEADERS += ../../engine/shell.h \
    ../../engine/scriptable/scriptableengine.h \
    ../../engine/scriptable/scriptableshellengine.h
SOURCES += ../../engine/shell.cpp \
    ../../engine/scriptable/scriptableengine.cpp \
    ../../engine/scriptable/scriptableshellengine.cpp
INCLUDEPATH += ../../engine
DEFINES += SCRIPTREPORTENGINE_NOLIBRARY
