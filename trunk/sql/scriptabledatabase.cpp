#include "scriptabledatabase.h"

#include <QtScript/QScriptEngine>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

#include "scriptableerror.h"
#include "scriptablequery.h"
#include "scriptablerecord.h"

ScriptableDatabase::ScriptableDatabase(QSqlDatabase &database, bool readonly, QObject *parent) :
    QObject(parent), QScriptable()
{
    m_db = new QSqlDatabase(database);
    m_readonly = readonly;
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
    return m_db->open();
}

bool ScriptableDatabase::open(const QString &user, const QString &password) {
    if (m_readonly) {
        context()->throwError(tr("Unable to change the user name and password for the connection '%1'").arg(m_db->connectionName()));
        return false;
    }
    return m_db->open(user, password);
}

void ScriptableDatabase::close() {
    if (m_readonly) {
        context()->throwError(tr("Unable to close the connection '%1'").arg(m_db->connectionName()));
        return;
    }
    m_db->close();
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

QStringList ScriptableDatabase::tables(QString type) const {
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
        context()->throwError(tr("%1 is not a valid table type.").arg(type));
        return QStringList();
    }

    return m_db->tables(tableType);
}

ScriptableRecord* ScriptableDatabase::record(const QString& tablename) const {
    QSqlRecord record = m_db->record(tablename);
    return new ScriptableRecord(record, engine());
}

ScriptableQuery* ScriptableDatabase::exec(const QString& query) const {
    QSqlQuery q = m_db->exec(query);
    return new ScriptableQuery(q, engine());
}

ScriptableQuery* ScriptableDatabase::query(const QString& query) const {
    QSqlQuery q(query, *m_db);
    return new ScriptableQuery(q, engine());
}

bool ScriptableDatabase::transaction() {
    return m_db->transaction();
}

bool ScriptableDatabase::commit() {
    return m_db->commit();
}

bool ScriptableDatabase::rollback() {
    return m_db->rollback();
}

void ScriptableDatabase::setDatabaseName(const QString &name) {
    if (m_readonly) {
        context()->throwError(tr("Unable to change the database name for the connection '%1'").arg(m_db->connectionName()));
        return;
    }
    m_db->setDatabaseName(name);
}

void ScriptableDatabase::setUserName(const QString &name) {
    if (m_readonly) {
        context()->throwError(tr("Unable to change the user name for the connection '%1'").arg(m_db->connectionName()));
        return;
    }
    m_db->setUserName(name);
}

void ScriptableDatabase::setPassword(const QString &password) {
    if (m_readonly) {
        context()->throwError(tr("Unable to change the password for the connection '%1'").arg(m_db->connectionName()));
        return;
    }
    m_db->setPassword(password);
}

void ScriptableDatabase::setHostName(const QString &host) {
    if (m_readonly) {
        context()->throwError(tr("Unable to change the host name for the connection '%1'").arg(m_db->connectionName()));
        return;
    }
    m_db->setHostName(host);
}

void ScriptableDatabase::setPort(int p) {
    if (m_readonly) {
        context()->throwError(tr("Unable to change the port for the connection '%1'").arg(m_db->connectionName()));
        return;
    }
    m_db->setPort(p);
}

void ScriptableDatabase::setConnectOptions(const QString &options) {
    if (m_readonly) {
        context()->throwError(tr("Unable to change the connect options for the connection '%1'").arg(m_db->connectionName()));
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

QString ScriptableDatabase::toString() {
    return tr("Database name: %1").arg(m_db->databaseName());
}
