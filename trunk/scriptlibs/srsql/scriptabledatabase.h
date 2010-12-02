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

#ifndef SCRIPTABLEDATABASE_H
#define SCRIPTABLEDATABASE_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QStringList>
#include <QtScript/QScriptable>

class QSqlDatabase;

class ScriptableError;
class ScriptableQuery;
class ScriptableRecord;

class ScriptableDatabase : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(bool isOpen READ isOpen)
    Q_PROPERTY(bool isOpenError READ isOpenError)
    Q_PROPERTY(ScriptableError* lastError READ lastError)
    Q_PROPERTY(bool isValid READ isValid)
    Q_PROPERTY(QString databaseName READ databaseName WRITE setDatabaseName)
    Q_PROPERTY(QString userName READ userName WRITE setUserName)
    Q_PROPERTY(QString password READ password WRITE setPassword)
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName)
    Q_PROPERTY(QString driverName READ driverName)
    Q_PROPERTY(int port READ port WRITE setPort)
    Q_PROPERTY(QString connectOptions READ connectOptions WRITE setConnectOptions)
    Q_PROPERTY(QString connectionName READ connectionName)
    Q_PROPERTY(bool autoThrow READ autoThrow WRITE setAutoThrow)

public:
    ScriptableDatabase(QSqlDatabase &database, bool readonly, bool autoThrow, QObject *parent = 0);
    ~ScriptableDatabase();

    QSqlDatabase *db() const;
    bool isReadOnly() const;

    Q_INVOKABLE bool open();
    Q_INVOKABLE bool open(const QString &user, const QString &password);
    Q_INVOKABLE void close();
    bool isOpen() const;
    bool isOpenError() const;
    ScriptableError* lastError() const;
    bool isValid() const;

    Q_INVOKABLE QStringList tables(QString type = QLatin1String("Tables"));
//    QSqlIndex primaryIndex(const QString& tablename);
    Q_INVOKABLE ScriptableRecord* record(const QString& tablename);
    Q_INVOKABLE ScriptableQuery* exec(const QString& query = QString());
    Q_INVOKABLE ScriptableQuery* query(const QString& query = QString());

    Q_INVOKABLE bool transaction();
    Q_INVOKABLE bool commit();
    Q_INVOKABLE bool rollback();

    void setDatabaseName(const QString &name);
    void setUserName(const QString &name);
    void setPassword(const QString &password);
    void setHostName(const QString &host);
    void setPort(int p);
    void setConnectOptions(const QString &options);
    QString databaseName() const;
    QString userName() const;
    QString password() const;
    QString hostName() const;
    QString driverName() const;
    int port() const;
    QString connectOptions() const;
    QString connectionName() const;
//    void setNumericalPrecisionPolicy(QSql::NumericalPrecisionPolicy precisionPolicy);
//    QSql::NumericalPrecisionPolicy numericalPrecisionPolicy() const;

    bool autoThrow() const;
    void setAutoThrow(bool autoThrow);

    Q_INVOKABLE QString toString();

private:
    inline void throwError() const;

private:
    QSqlDatabase* m_db;
    bool m_readonly;
    bool m_autoThrow;
};

Q_DECLARE_METATYPE(ScriptableDatabase*)

#endif // SCRIPTABLEDATABASE_H
