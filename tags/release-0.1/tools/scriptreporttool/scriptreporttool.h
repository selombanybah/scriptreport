/*
 * Copyright 2010 and beyond, Juan Luis Paz
 *
 * This file is part of Script Report.
 *
 * Script Report is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Script Report is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Script Report.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCRIPTREPORTTOOL_H
#define SCRIPTREPORTTOOL_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

class QFile;
class QTextStream;
class QPrinter;

class ScriptReport;

class ScriptReportTool : public QObject
{
    Q_OBJECT

public:
    explicit ScriptReportTool(QObject *parent = 0);
    ~ScriptReportTool();

    bool init(int &retunrCode);
    void printHelp();

public slots:
    void run();
    void runPreview();
    void compile();

private:
    QString m_fileName;
    QString m_previousScript;
    QStringList m_scriptArguments;
    QString m_compiledFileNane;
    bool m_preview;

    QFile *m_inFile;
    QFile *m_outFile;
    QTextStream *m_in;
    QTextStream *m_err;
    QTextStream *m_out;

    bool m_editing;
    bool m_debugging;
    bool m_printError;
};

#endif // SCRIPTREPORTTOOL_H
