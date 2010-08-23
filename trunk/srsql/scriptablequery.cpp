#include "scriptablequery.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

#include <QtScript/QScriptEngine>

#include "scriptableerror.h"
#include "scriptablerecord.h"

void ScriptableQuery::throwError() const {
    if (m_autoThrow) {
        QSqlError error = m_query->lastError();
        if (error.isValid()) {
            context()->throwError(error.text());
        }
    }
}

ScriptableQuery::ScriptableQuery(QSqlQuery &query, bool autoThrow, QObject *parent) :
    QObject(parent), QScriptable(), m_autoThrow(autoThrow)
{
    m_query = new QSqlQuery(query);
}

ScriptableQuery::~ScriptableQuery() {
    delete m_query;
}

bool ScriptableQuery::isValid() const {
    return m_query->isValid();
}

bool ScriptableQuery::isActive() const {
    return m_query->isActive();
}

bool ScriptableQuery::isNull(const QString& field) const {
    return m_query->record().isNull(field);
}

bool ScriptableQuery::isNull(int field) const {
    return m_query->isNull(field);
}

int ScriptableQuery::at() const {
    return m_query->at();
}

QString ScriptableQuery::lastQuery() const {
    return m_query->lastQuery();
}

int ScriptableQuery::numRowsAffected() const {
    return m_query->numRowsAffected();
}

ScriptableError* ScriptableQuery::lastError() const {
    QSqlError error = m_query->lastError();
    if (error.isValid()) {
        return new ScriptableError(error, this->engine());
    } else {
        return 0;
    }
}

bool ScriptableQuery::isSelect() const {
    return m_query->isSelect();
}

int ScriptableQuery::size() const {
    return m_query->size();
}

bool ScriptableQuery::isForwardOnly() const {
    return m_query->isForwardOnly();
}

void ScriptableQuery::setForwardOnly(bool forward) {
    m_query->setForwardOnly(forward);
}

ScriptableRecord* ScriptableQuery::record() const {
    QSqlRecord record = m_query->record();
    return new ScriptableRecord(record, m_autoThrow, engine());
}

bool ScriptableQuery::exec(const QString& query) {
    bool result = m_query->exec(query);
    throwError();
    return result;
}

QVariant ScriptableQuery::value(const QString& field) const {
    QVariant result = m_query->record().value(field);
    throwError();
    if (m_autoThrow) {
        QSqlError error = m_query->lastError();
        if (error.isValid()) {
            context()->throwError(error.text());
        }
        if (!result.isValid()) {
            context()->throwError(tr("Invalid field of name: '%1'").arg(field));
        }
    }
    return result;
}

QVariant ScriptableQuery::value(int i) const {
    QVariant result = m_query->value(i);
    throwError();
    if (m_autoThrow) {
        QSqlError error = m_query->lastError();
        if (error.isValid()) {
            context()->throwError(error.text());
        }
        if (!result.isValid()) {
            context()->throwError(tr("Invalid field of index: '%1'").arg(i));
        }
    }
    return result;
}

bool ScriptableQuery::seek(int i, bool relative) {
    bool result = m_query->seek(i, relative);
    throwError();
    return result;
}

bool ScriptableQuery::next() {
    bool result = m_query->next();
    throwError();
    return result;
}

bool ScriptableQuery::previous() {
    bool result = m_query->previous();
    throwError();
    return result;
}

bool ScriptableQuery::first() {
    bool result = m_query->first();
    throwError();
    return result;
}

bool ScriptableQuery::last() {
    bool result = m_query->last();
    throwError();
    return result;
}

void ScriptableQuery::clear() {
    m_query->clear();
    throwError();
}

bool ScriptableQuery::exec() {
    bool result = m_query->exec();
    throwError();
    return result;
}

bool ScriptableQuery::execBatch(QString mode) {
    QSqlQuery::BatchExecutionMode executionMode;
    if (mode == QLatin1String("ValuesAsRows")) {
        executionMode = QSqlQuery::ValuesAsRows;
    } else if (mode == QLatin1String("ValuesAsColumns")) {
        executionMode = QSqlQuery::ValuesAsColumns;
    } else {
        context()->throwError(tr("%1 is not a valid batch execution mode.").arg(mode));
        return false;
    }

    bool result = m_query->execBatch(executionMode);
    throwError();
    return result;
}


bool ScriptableQuery::prepare(const QString& query) {
    bool result = m_query->prepare(query);
    throwError();
    return result;
}

void ScriptableQuery::bindValue(const QString& placeholder, const QVariant& val) {
    m_query->bindValue(placeholder, val);
    throwError();
}

void ScriptableQuery::bindValue(int pos, const QVariant& val) {
    m_query->bindValue(pos, val);
    throwError();
}

void ScriptableQuery::addBindValue(const QVariant& val) {
    m_query->addBindValue(val);
    throwError();
}

QVariant ScriptableQuery::boundValue(const QString& placeholder) const {
    QVariant result = m_query->boundValue(placeholder);
    throwError();
    return result;
}

QVariant ScriptableQuery::boundValue(int pos) const {
    QVariant result = m_query->boundValue(pos);
    throwError();
    return result;
}

QMap<QString, QVariant> ScriptableQuery::boundValues() const {
    QMap<QString, QVariant> result = m_query->boundValues();
    throwError();
    return result;
}

QString ScriptableQuery::executedQuery() const {
    return m_query->executedQuery();
}

QVariant ScriptableQuery::lastInsertId() const {
    QVariant result = m_query->lastInsertId();
    throwError();
    return result;
}

void ScriptableQuery::finish() {
    m_query->finish();
    throwError();
}

bool ScriptableQuery::nextResult() {
    bool result = m_query->nextResult();
    throwError();
    return result;
}

bool ScriptableQuery::autoThrow() const {
    return m_autoThrow;
}

void ScriptableQuery::setAutoThrow(bool autoThrow) {
    m_autoThrow = autoThrow;
}
