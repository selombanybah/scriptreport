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
