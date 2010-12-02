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

#include "scriptableerror.h"

#include <QtSql/QSqlError>

ScriptableError::ScriptableError(QSqlError &error, QObject *parent) :
    QObject(parent), QScriptable()
{
    m_error = new QSqlError(error);
}

ScriptableError::~ScriptableError() {
    delete m_error;
}

QString ScriptableError::databaseText() const {
    return m_error->databaseText();
}

QString ScriptableError::driverText() const {
    return m_error->driverText();
}

int ScriptableError::number() const {
    return m_error->number();
}

QString ScriptableError::text() const {
    return m_error->text();
}

QString ScriptableError::type() const {
    switch(m_error->type()) {
    case QSqlError::NoError:
        return QString::fromLatin1("NoError");
    case QSqlError::ConnectionError:
        return QString::fromLatin1("ConnectionError");
    case QSqlError::StatementError:
        return QString::fromLatin1("StatementError");
    case QSqlError::TransactionError:
        return QString::fromLatin1("TransactionError");
    case QSqlError::UnknownError:
        return QString::fromLatin1("UnknownError");
    default:
        return QString::fromLatin1("UnknownError");
    }
}

QString ScriptableError::toString() const {
    return m_error->text();
}
