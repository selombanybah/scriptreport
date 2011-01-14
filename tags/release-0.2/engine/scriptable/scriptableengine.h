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

#ifndef SCRIPTABLEENGINE_H
#define SCRIPTABLEENGINE_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>

class ScriptableEngine : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QStringList availableExtensions READ availableExtensions)
    Q_PROPERTY(QStringList importedExtensions READ importedExtensions)
    Q_PROPERTY(QString qtVersion READ qtVersion)
    Q_PROPERTY(QString srVersion READ srVersion)
    Q_PROPERTY(QStringList arguments READ arguments)

public:
    explicit ScriptableEngine(QObject *parent = 0);

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

#endif // SCRIPTABLEENGINE_H
