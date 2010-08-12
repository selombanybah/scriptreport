#include "scriptablerecord.h"

#include <QtSql/QSqlRecord>

ScriptableRecord::ScriptableRecord(QSqlRecord &record, QObject *parent) :
    QObject(parent), QScriptable()
{
    m_record = new QSqlRecord(record);
}

ScriptableRecord::~ScriptableRecord() {
    delete m_record;
}

QVariant ScriptableRecord::value(int i) const {
    return m_record->value(i);
}

QVariant ScriptableRecord::value(const QString& name) const {
    return m_record->value(name);
}

bool ScriptableRecord::isNull(int i) const {
    return m_record->isNull(i);
}

bool ScriptableRecord::isNull(const QString& name) const {
    return m_record->isNull(name);
}

int ScriptableRecord::indexOf(const QString &name) const {
    return m_record->indexOf(name);
}

QString ScriptableRecord::fieldName(int i) const {
    return m_record->fieldName(i);
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
