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

#ifndef SCRIPTABLESHELLENGINE_H
#define SCRIPTABLESHELLENGINE_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>

#include "shell.h"

//TODO: extends from ScriptableEngine, problem: we neew use QScriptEngine::ExcludeSuperClassContents for hide QObject members
class ScriptableShellEngine : public QObject, public QScriptable /*public ScriptableEngine*/
{
    Q_OBJECT
    /*************************************************************************
     *** From ScriptableEngine ***********************************************
     *************************************************************************/
    Q_PROPERTY(QStringList availableExtensions READ availableExtensions)
    Q_PROPERTY(QStringList importedExtensions READ importedExtensions)
    Q_PROPERTY(QString qtVersion READ qtVersion)
    Q_PROPERTY(QString srVersion READ srVersion)
    Q_PROPERTY(QStringList arguments READ arguments)

public:
    explicit ScriptableShellEngine(Shell *shell, QObject *parent = 0);

    Q_INVOKABLE void help();

    Q_INVOKABLE void error();
    Q_INVOKABLE void print();
    Q_INVOKABLE QString read();

    Q_INVOKABLE void exit(int returnCode = 0);
    Q_INVOKABLE void quit();

private:
    Shell *m_shell;

/*****************************************************************************
 *** From ScriptableEngine ***************************************************
 *****************************************************************************/

public:
    QStringList availableExtensions() const;
    QStringList importedExtensions() const;
    QString qtVersion() const;
    QString srVersion() const;
    QStringList arguments() const;
    void setArguments(QStringList arguments);

    Q_INVOKABLE void importExtension();

    Q_INVOKABLE QString readFile(QString name);
    Q_INVOKABLE void load();
    Q_INVOKABLE int runCommand();

    Q_INVOKABLE QString getFromEnvironment(QString name);

    Q_INVOKABLE void installApplicationTranslatorFunctions(const QScriptValue &object = QScriptValue());

private:
    QStringList m_arguments;

};

#endif // SCRIPTABLESHELLENGINE_H
