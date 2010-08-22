# -------------------------------------------------
# Project created by QtCreator 2010-04-04T16:28:35
# -------------------------------------------------
QT += script \
    scripttools
TARGET = scriptreporteditor
TEMPLATE = app
DESTDIR = ../compiled
win32 {
    LIBS += ../compiled/scriptreportengine0.dll
}
unix {
    LIBS += ../compiled/libscriptreportengine.so.0
}
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

DEFINES += QT_NO_CAST_FROM_ASCII \
    QT_NO_CAST_TO_ASCII

# The application version
VERSION = 0.1

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
