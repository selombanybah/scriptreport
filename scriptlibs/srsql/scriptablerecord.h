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

#ifndef SCRIPTABLERECORD_H
#define SCRIPTABLERECORD_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QMetaType>
#include <QtCore/QStringList>
#include <QtScript/QScriptable>

class QSqlRecord;

class ScriptableRecord : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty)
    Q_PROPERTY(int count READ count)
    Q_PROPERTY(bool autoThrow READ autoThrow WRITE setAutoThrow)

public:
    explicit ScriptableRecord(QSqlRecord &record, bool autoThrow, QObject *parent = 0);
    ~ScriptableRecord();

    Q_INVOKABLE QVariant value(int i) const;
    Q_INVOKABLE QVariant value(const QString& name) const;

    Q_INVOKABLE bool isNull(int i) const;
    Q_INVOKABLE bool isNull(const QString& name) const;

    Q_INVOKABLE int indexOf(const QString &name) const;
    Q_INVOKABLE QString fieldName(int i) const;

//    QSqlField field(int i) const;
//    QSqlField field(const QString &name) const;

    Q_INVOKABLE bool isGenerated(int i) const;
    Q_INVOKABLE bool isGenerated(const QString& name) const;

    bool isEmpty() const;
    Q_INVOKABLE bool contains(const QString& name) const;
    int count() const;

    bool autoThrow() const;
    void setAutoThrow(bool autoThrow);

private:
    QSqlRecord *m_record;
    bool m_autoThrow;
};

Q_DECLARE_METATYPE(ScriptableRecord*)

#endif // SCRIPTABLERECORD_H
