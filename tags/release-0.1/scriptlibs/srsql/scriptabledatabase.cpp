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

#include "scriptabledatabase.h"

#include <QtScript/QScriptEngine>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

#include "scriptableerror.h"
#include "scriptablequery.h"
#include "scriptablerecord.h"

void ScriptableDatabase::throwError() const {
    if (m_autoThrow) {
        QSqlError error = m_db->lastError();
        if (error.isValid()) {
            context()->throwError(error.text());
        }
    }
}

ScriptableDatabase::ScriptableDatabase(QSqlDatabase &database, bool readonly, bool autoThrow, QObject *parent) :
    QObject(parent), QScriptable(), m_readonly(readonly), m_autoThrow(autoThrow)
{
    m_db = new QSqlDatabase(database);
}


ScriptableDatabase::~ScriptableDatabase() {
    delete m_db;
}

QSqlDatabase* ScriptableDatabase::db() const {
    return m_db;
}


bool ScriptableDatabase::isReadOnly() const {
    return m_readonly;
}

bool ScriptableDatabase::open() {
    bool result = m_db->open();
    throwError();
    return result;
}

bool ScriptableDatabase::open(const QString &user, const QString &password) {
    if (m_readonly) {
        if (m_autoThrow) {
            context()->throwError(tr("Unable to change the user name and password for the connection '%1'").arg(m_db->connectionName()));
        }
        return false;
    }
    bool result = m_db->open(user, password);
    throwError();
    return result;
}

void ScriptableDatabase::close() {
    if (m_readonly) {
        if (m_autoThrow) {
            context()->throwError(tr("Unable to close the connection '%1'").arg(m_db->connectionName()));
        }
        return;
    }
    m_db->close();
    throwError();
}

bool ScriptableDatabase::isOpen() const {
    return m_db->isOpen();
}

bool ScriptableDatabase::isOpenError() const {
    return m_db->isOpenError();
}

ScriptableError* ScriptableDatabase::lastError() const {
    QSqlError error = m_db->lastError();
    if (error.isValid()) {
        return new ScriptableError(error, this->engine());
    } else {
        return 0;
    }
}

bool ScriptableDatabase::isValid() const {
    return m_db->isValid();
}

QStringList ScriptableDatabase::tables(QString type) {
    QSql::TableType tableType;

    if (type == QLatin1String("Tables")) {
        tableType = QSql::Tables;
    } else if (type == QLatin1String("SystemTables")) {
        tableType = QSql::SystemTables;
    } else if (type == QLatin1String("Views")) {
        tableType = QSql::Views;
    } else if (type == QLatin1String("AllTables")) {
        tableType = QSql::AllTables;
    } else {
        if (m_autoThrow) {
            context()->throwError(tr("%1 is not a valid table type.").arg(type));
        }
        return QStringList();
    }

    QStringList result = m_db->tables(tableType);
    throwError();
    return result;
}

ScriptableRecord* ScriptableDatabase::record(const QString& tablename) {
    QSqlRecord record = m_db->record(tablename);
    throwError();
    return new ScriptableRecord(record, m_autoThrow, engine());
}

ScriptableQuery* ScriptableDatabase::exec(const QString& query) {
    QSqlQuery q = m_db->exec(query);
    if (m_autoThrow) {
        QSqlError error = m_db->lastError();
        if (error.isValid()) {
            context()->throwError(error.text());
        }
    }
    throwError();
    // this must be the parent of the ScriptableQuery for prevent a message from qt
    // when the database is remove in ScriptableSql
    return new ScriptableQuery(q, m_autoThrow, this);
}

ScriptableQuery* ScriptableDatabase::query(const QString& query) {
    QSqlQuery q(query, *m_db);
    // this must be the parent of the ScriptableQuery for prevent a message from qt
    // when the database is remove in ScriptableSql
    return new ScriptableQuery(q, m_autoThrow, this);
}

bool ScriptableDatabase::transaction() {
    bool result = m_db->transaction();
    throwError();
    return result;
}

bool ScriptableDatabase::commit() {
    bool result = m_db->commit();
    throwError();
    return result;
}

bool ScriptableDatabase::rollback() {
    bool result = m_db->rollback();
    throwError();
    return result;
}

void ScriptableDatabase::setDatabaseName(const QString &name) {
    if (m_readonly) {
        if (m_autoThrow) {
            context()->throwError(tr("Unable to change the database name for the connection '%1'").arg(m_db->connectionName()));
        }
        return;
    }
    m_db->setDatabaseName(name);
}

void ScriptableDatabase::setUserName(const QString &name) {
    if (m_readonly) {
        if (m_autoThrow) {
            context()->throwError(tr("Unable to change the user name for the connection '%1'").arg(m_db->connectionName()));
        }
        return;
    }
    m_db->setUserName(name);
}

void ScriptableDatabase::setPassword(const QString &password) {
    if (m_readonly) {
        if (m_autoThrow) {
            context()->throwError(tr("Unable to change the password for the connection '%1'").arg(m_db->connectionName()));
        }
        return;
    }
    m_db->setPassword(password);
}

void ScriptableDatabase::setHostName(const QString &host) {
    if (m_readonly) {
        if (m_autoThrow) {
            context()->throwError(tr("Unable to change the host name for the connection '%1'").arg(m_db->connectionName()));
        }
        return;
    }
    m_db->setHostName(host);
}

void ScriptableDatabase::setPort(int p) {
    if (m_readonly) {
        if (m_autoThrow) {
            context()->throwError(tr("Unable to change the port for the connection '%1'").arg(m_db->connectionName()));
        }
        return;
    }
    m_db->setPort(p);
}

void ScriptableDatabase::setConnectOptions(const QString &options) {
    if (m_readonly) {
        if (m_autoThrow) {
            context()->throwError(tr("Unable to change the connect options for the connection '%1'").arg(m_db->connectionName()));
        }
        return;
    }
    return m_db->setConnectOptions(options);
}

QString ScriptableDatabase::databaseName() const {
    return m_db->databaseName();
}

QString ScriptableDatabase::userName() const {
    return m_db->userName();
}

QString ScriptableDatabase::password() const {
    if (m_readonly) {
        return QString();
    }
    return m_db->password();
}

QString ScriptableDatabase::hostName() const {
    return m_db->hostName();
}

QString ScriptableDatabase::driverName() const {
    return m_db->driverName();
}

int ScriptableDatabase::port() const {
    return m_db->port();
}

QString ScriptableDatabase::connectOptions() const {
    return m_db->connectOptions();
}

QString ScriptableDatabase::connectionName() const {
    return m_db->connectionName();
}

bool ScriptableDatabase::autoThrow() const {
    return m_autoThrow;
}

void ScriptableDatabase::setAutoThrow(bool autoThrow) {
    m_autoThrow = autoThrow;
}

QString ScriptableDatabase::toString() {
    return tr("Database name: %1").arg(m_db->databaseName());
}
