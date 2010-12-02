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

#ifndef SCRIPTABLESQL_H
#define SCRIPTABLESQL_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtScript/QScriptable>
#include <QtCore/QMap>

class QScriptEngine;
class ScriptableDatabase;

class ScriptableSql : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QStringList connectionNames READ connectionNames)
    Q_PROPERTY(QStringList drivers READ drivers)
    Q_PROPERTY(bool autoThrow READ autoThrow WRITE setAutoThrow)

public:
    explicit ScriptableSql(QObject *parent = 0);
    ~ScriptableSql();

    QStringList connectionNames();
    QStringList drivers();

    bool autoThrow() const;
    void setAutoThrow(bool autoThrow);

    Q_INVOKABLE bool contains(const QString &connectionName = QLatin1String(defaultConnection));
    Q_INVOKABLE bool isDriverAvailable(const QString &name);
    Q_INVOKABLE void removeDatabase(const QString &connectionName);

    Q_INVOKABLE ScriptableDatabase* addDatabase(const QString &type, const QString &connectionName = QLatin1String(defaultConnection));
    Q_INVOKABLE ScriptableDatabase* cloneDatabase(const ScriptableDatabase &other, const QString &connectionName);
    Q_INVOKABLE ScriptableDatabase* database(const QString &connectionName = QLatin1String(defaultConnection), bool open = true);

private:
    QT_STATIC_CONST char *defaultConnection;
    QMap<QString, ScriptableDatabase*> m_connections;
    bool m_autoThrow;
};

#endif // SCRIPTABLESQL_H
