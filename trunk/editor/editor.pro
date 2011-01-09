#
# Copyright 2010 and beyond, Juan Luis Paz
#
# This file is part of Script Report.
#
# Script Report is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Script Report is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Script Report.  If not, see <http://www.gnu.org/licenses/>.
#

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
TRANSLATIONS = translations/scriptreporteditor_es.ts \
    translations/scriptreporteditor_ru.ts

#release translations
isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt4 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}

lrelease.input = TRANSLATIONS
lrelease.output = ../compiled/translations/${QMAKE_FILE_BASE}.qm
lrelease.commands = $$QMAKE_LRELEASE -silent ${QMAKE_FILE_IN} -qm ../compiled/translations/${QMAKE_FILE_BASE}.qm
lrelease.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

#for add a dependency
#lrelease.depends = lupdate
