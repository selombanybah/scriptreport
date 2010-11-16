# -------------------------------------------------
# Project created by QtCreator 2010-04-04T16:28:35
# -------------------------------------------------
include(../scriptreport.pri)
QT += script \
    scripttools
TARGET = scriptreporteditor
TEMPLATE = app
DESTDIR = ../compiled
win32:LIBS += -L../compiled \
    -lscriptreportengine$${VERSION_MAJOR}
else:LIBS += -L../compiled \
    -lscriptreportengine
unix:LIBS += -Wl,-rpath,.
INCLUDEPATH += ../includes
SOURCES += main.cpp \
    mainwindow.cpp \
    previewtransformer.cpp \
    edithandler.cpp \
    previewhandler.cpp \
    plaintexteditwithlinenumber.cpp \
    syntaxhighlighter.cpp \
    textdocumentsyntaxhighlighter.cpp \
    shellform.cpp \
    shellcontroller.cpp
HEADERS += mainwindow.h \
    previewtransformer.h \
    extendeduimainwindow.h \
    edithandler.h \
    previewhandler.h \
    plaintexteditwithlinenumber.h \
    syntaxhighlighter.h \
    textdocumentsyntaxhighlighter.h \
    shellform.h \
    shellcontroller.h
FORMS += mainwindow.ui \
    shellform.ui
RESOURCES += scriptreporteditor.qrc
