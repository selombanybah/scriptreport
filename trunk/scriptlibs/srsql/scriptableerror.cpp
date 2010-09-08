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
