# -------------------------------------------------
# Project created by QtCreator 2010-04-04T16:28:35
# -------------------------------------------------
include(../scriptreport.pri)
QT += script \
    scripttools
TARGET = scriptreporteditor
TEMPLATE = app
DESTDIR = ../compiled
LIBS += -L../compiled -lscriptreportengine
unix:LIBS += -Wl,-rpath,.
INCLUDEPATH += ../includes
SOURCES += main.cpp \
    mainwindow.cpp \
    previewtransformer.cpp \
    edithandler.cpp \
    previewhandler.cpp \
    plaintexteditwithlinenumber.cpp \
    syntaxhighlighter.cpp \
    textdocumentsyntaxhighlighter.cpp
HEADERS += mainwindow.h \
    previewtransformer.h \
    extendeduimainwindow.h \
    edithandler.h \
    previewhandler.h \
    plaintexteditwithlinenumber.h \
    syntaxhighlighter.h \
    textdocumentsyntaxhighlighter.h
FORMS += mainwindow.ui
RESOURCES += scriptreporteditor.qrc
