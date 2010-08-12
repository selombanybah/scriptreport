#include "scriptablequery.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

#include <QtScript/QScriptEngine>

#include "scriptableerror.h"
#include "scriptablerecord.h"

ScriptableQuery::ScriptableQuery(QSqlQuery &query, QObject *parent) :
    QObject(parent), QScriptable()
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
    return new ScriptableRecord(record, engine());
}

bool ScriptableQuery::exec(const QString& query) {
    return m_query->exec(query);
}

QVariant ScriptableQuery::value(int i) const {
    return m_query->value(i);
}

bool ScriptableQuery::seek(int i, bool relative) {
    return m_query->seek(i, relative);
}

bool ScriptableQuery::next() {
    return m_query->next();
}

bool ScriptableQuery::previous() {
    return m_query->previous();
}

bool ScriptableQuery::first() {
    return m_query->first();
}

bool ScriptableQuery::last() {
    return m_query->last();
}

void ScriptableQuery::clear() {
    return m_query->clear();
}

bool ScriptableQuery::exec() {
    return m_query->exec();
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

    return m_query->execBatch(executionMode);
}


bool ScriptableQuery::prepare(const QString& query) {
    return m_query->prepare(query);
}

void ScriptableQuery::bindValue(const QString& placeholder, const QVariant& val) {
    m_query->bindValue(placeholder, val);
}

void ScriptableQuery::bindValue(int pos, const QVariant& val) {
    m_query->bindValue(pos, val);
}

void ScriptableQuery::addBindValue(const QVariant& val) {
    m_query->addBindValue(val);
}

QVariant ScriptableQuery::boundValue(const QString& placeholder) const {
    return m_query->boundValue(placeholder);
}

QVariant ScriptableQuery::boundValue(int pos) const {
    return m_query->boundValue(pos);
}

QMap<QString, QVariant> ScriptableQuery::boundValues() const {
    return m_query->boundValues();
}

QString ScriptableQuery::executedQuery() const {
    return m_query->executedQuery();
}

QVariant ScriptableQuery::lastInsertId() const {
    return m_query->lastInsertId();
}

void ScriptableQuery::finish() {
    m_query->finish();
}

bool ScriptableQuery::nextResult() {
    return m_query->nextResult();
}
