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

#include "scriptablerecord.h"

#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtScript/QScriptEngine>

ScriptableRecord::ScriptableRecord(QSqlRecord &record, bool autoThrow, QObject *parent) :
    QObject(parent), QScriptable(), m_autoThrow(autoThrow)
{
    m_record = new QSqlRecord(record);
}

ScriptableRecord::~ScriptableRecord() {
    delete m_record;
}

QVariant ScriptableRecord::value(int i) const {
    QVariant result = m_record->value(i);
    if (m_autoThrow) {
        if (!result.isValid()) {
            context()->throwError(tr("Invalid field of index: '%1'").arg(i));
        }
    }
    return result;
}

QVariant ScriptableRecord::value(const QString& name) const {
    QVariant result = m_record->value(name);
    if (m_autoThrow) {
        if (!result.isValid()) {
            context()->throwError(tr("Invalid field of name: '%1'").arg(name));
        }
    }
    return result;
}

bool ScriptableRecord::isNull(int i) const {
    return m_record->isNull(i);
}

bool ScriptableRecord::isNull(const QString& name) const {
    return m_record->isNull(name);
}

int ScriptableRecord::indexOf(const QString &name) const {
    int result = m_record->indexOf(name);
    if (m_autoThrow) {
        if (result == -1) {
            context()->throwError(tr("Invalid field of name: '%1'").arg(name));
        }
    }
    return result;
}

QString ScriptableRecord::fieldName(int i) const {
    QString result = m_record->fieldName(i);
    if (m_autoThrow) {
        if (result.isEmpty()) {
            context()->throwError(tr("Invalid field of index: '%1'").arg(i));
        }
    }
    return result;
}

bool ScriptableRecord::isGenerated(int i) const {
    return m_record->isGenerated(i);
}

bool ScriptableRecord::isGenerated(const QString& name) const {
    return m_record->isGenerated(name);
}

bool ScriptableRecord::isEmpty() const {
    return m_record->isEmpty();
}

bool ScriptableRecord::contains(const QString& name) const {
    return m_record->contains(name);
}

int ScriptableRecord::count() const {
    return m_record->count();
}

bool ScriptableRecord::autoThrow() const {
    return m_autoThrow;
}

void ScriptableRecord::setAutoThrow(bool autoThrow) {
    m_autoThrow = autoThrow;
}
