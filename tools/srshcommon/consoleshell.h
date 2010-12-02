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

#ifndef CONSOLESHELL_H
#define CONSOLESHELL_H

#include <shell.h>

class QFile;
class QTextStream;

class ConsoleShell : public Shell
{
    Q_OBJECT

public:
    explicit ConsoleShell(QObject *parent = 0);
    ~ConsoleShell();

    bool init(int &retunrCode);

    //overrides:
    QString readCommand();
    QString readAll(int &finalLineNumber);
    QString readSentence(int &finalLineNumber);
    void printForReadCommand(const QScriptValue &message, bool last = true);
    void printOut(const QScriptValue &result, bool last = true);
    void printErr(const QScriptValue &error, bool last = true);
    void printUncaughtException(const QScriptValue &exception);
    void helpCommand();

private slots:
    void close(int returnCode);

private:
    void printPrompt();
    void printContinue();
    bool isStdinTty();
    QString readLineTty(bool isContinue);

private:
    enum ShellMode { None, Interactive, Batch, Quiet };
    QFile *m_inFile;
    QTextStream *m_in;
    QTextStream *m_err;
    QTextStream *m_out;
    ShellMode m_mode;
    bool m_isInATty;
    QString m_script;
};

#endif // CONSOLESHELL_H
