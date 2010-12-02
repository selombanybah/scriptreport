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

#ifndef SCRIPTABLEERROR_H
#define SCRIPTABLEERROR_H

#include <QObject>
#include <QtCore/QMetaType>
#include <QtScript/QScriptable>

class QSqlError;

class ScriptableError : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QString databaseText READ databaseText)
    Q_PROPERTY(QString driverText READ driverText)
    Q_PROPERTY(int number READ number)
    Q_PROPERTY(QString text READ text)
    Q_PROPERTY(QString type READ type)

public:
    explicit ScriptableError(QSqlError &error, QObject *parent = 0);
    ~ScriptableError();

    QString databaseText() const;
    QString driverText() const;
    int number() const;
    QString text() const;
    QString type() const;
    Q_INVOKABLE QString toString() const;

private:
    QSqlError *m_error;

};

Q_DECLARE_METATYPE(ScriptableError*)

#endif // SCRIPTABLEERROR_H
