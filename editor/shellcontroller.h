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

#ifndef SHELLCONTROLLER_H
#define SHELLCONTROLLER_H

#include <ScriptReport/Shell>

class QWidget;

class ShellController : public Shell
{
    Q_OBJECT
    Q_PROPERTY (QString textForRead READ textForRead WRITE setTextForRead)

public:
    explicit ShellController(QWidget *parent = 0);

    //overrides:
    QString readCommand();
    QString readAll(int &finalLineNumber);
    QString readSentence(int &finalLineNumber);
    void printForReadCommand(const QScriptValue &message, bool last = true);
    void printOut(const QScriptValue &result, bool last = true);
    void printErr(const QScriptValue &error, bool last = true);
    void printUncaughtException(const QScriptValue &exception);
    void helpCommand();

    QString textForRead() const;
    void setTextForRead(QString readText);

signals:
    void write(QString text);

private:
    QString m_buffer;
    QString m_readBuffer;
    QWidget *m_widget;
};

#endif // SHELLCONTROLLER_H
