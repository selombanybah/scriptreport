include(../../scriptreport.pri)
QT += script
CONFIG += console
CONFIG -= app_bundle
TARGET = scriptreporttool
TEMPLATE = app
DESTDIR = ../../compiled
win32 {
    LIBS += -L../../compiled -lscriptreportengine$${VERSION_MAJOR}
} else {
    LIBS += -L../../compiled -lscriptreportengine
}
unix:LIBS += -Wl,-rpath,.
INCLUDEPATH += ../../includes
DEFINES += APP_NAME=\\\"scriptreporttool\\\"
SOURCES += main.cpp \
    scriptreporttool.cpp
HEADERS += scriptreporttool.h
