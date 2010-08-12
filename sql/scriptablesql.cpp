#include "scriptablesql.h"

#include <QtCore/QMapIterator>
#include <QtSql/QSqlDatabase>
#include <QtScript/QScriptEngine>

#include "scriptabledatabase.h"

QT_STATIC_CONST_IMPL char *ScriptableSql::defaultConnection = "qt_sql_default_connection";

ScriptableSql::ScriptableSql(QObject *parent) :
    QObject(parent), QScriptable()
{
}

ScriptableSql::~ScriptableSql() {
    QMapIterator<QString, ScriptableDatabase *> i(m_connections);
    while (i.hasNext()) {
        i.next();
        if (!i.value()->isReadOnly()) {
            QString name = i.value()->connectionName();
            delete i.value(); // don't worry, m_connections will be deleted too
                              // this prevent a message fron qt where the database is in use
            QSqlDatabase::removeDatabase(name);
        }
    }
}

QStringList ScriptableSql::connectionNames() {
    return QSqlDatabase::connectionNames();
}

QStringList ScriptableSql::drivers() {
    return QSqlDatabase::drivers();
}

bool ScriptableSql::contains(const QString &connectionName) {
    return QSqlDatabase::contains(connectionName);
}

bool ScriptableSql::isDriverAvailable(const QString &name) {
    return QSqlDatabase::isDriverAvailable(name);
}

void ScriptableSql::removeDatabase(const QString &connectionName) {
    ScriptableDatabase* old = m_connections.take(connectionName);
    if (old) {
        if (!old->isReadOnly()) {
            QString name = old->connectionName();
            delete old;
            QSqlDatabase::removeDatabase(name);
        }
    }
}

ScriptableDatabase* ScriptableSql::addDatabase(const QString &type, const QString &connectionName) {
    ScriptableDatabase* old = m_connections.take(connectionName);
    if (old) {
        if (!old->isReadOnly()) {
            delete old;
        } else {
            context()->throwError(tr("The connection '%1' already exists and can not be replaced").arg(connectionName));
            return 0;
        }
    } else if (QSqlDatabase::contains(connectionName)) {
        context()->throwError(tr("The connection '%1' already exists and can not be replaced").arg(connectionName));
        return 0;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(type, connectionName);
    ScriptableDatabase *result = new ScriptableDatabase(db, false, this);
    m_connections.insert(connectionName, result);
    return result;
}

ScriptableDatabase* ScriptableSql::cloneDatabase(const ScriptableDatabase &other, const QString &connectionName) {
    ScriptableDatabase* old = m_connections.take(connectionName);
    if (old) {
        if (!old->isReadOnly()) {
            delete old;
        } else {
            context()->throwError(tr("The connection '%1' already exists and can not be replaced").arg(connectionName));
            return 0;
        }
    } else if (QSqlDatabase::contains(connectionName)) {
        context()->throwError(tr("The connection '%1' already exists and can not be replaced").arg(connectionName));
        return 0;
    }

    QSqlDatabase *db1 = other.db();
    QSqlDatabase db2 = QSqlDatabase::cloneDatabase(*db1, connectionName);
    ScriptableDatabase* result = new ScriptableDatabase(db2, false, this);
    m_connections.insert(connectionName, result);
    return result;
}

ScriptableDatabase* ScriptableSql::database(const QString &connectionName, bool open) {
    ScriptableDatabase* result = m_connections[connectionName];
    if (result) {
        return result;
    }

    QSqlDatabase db = QSqlDatabase::database(connectionName, open);
    result = new ScriptableDatabase(db, true, this);
    m_connections.insert(connectionName, result);
    return result;
}
