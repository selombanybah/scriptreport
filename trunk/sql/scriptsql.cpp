#include "scriptsql.h"

#include <QtScript/QScriptEngine>

#include "scriptablesql.h"
#include "scriptabledatabase.h"
#include "scriptableerror.h"
#include "scriptablequery.h"
#include "scriptablerecord.h"

/*
 * Statics
 */

static QScriptValue databaseToScriptValue(QScriptEngine *engine, ScriptableDatabase* const &in) {
    return engine->newQObject(in, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
}

static void databaseFromScriptValue(const QScriptValue &object, ScriptableDatabase* &out) {
    out = qobject_cast<ScriptableDatabase*>(object.toQObject());
}

static QScriptValue errorToScriptValue(QScriptEngine *engine, ScriptableError* const &in) {
    return engine->newQObject(in, QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
}

static void errorFromScriptValue(const QScriptValue &object, ScriptableError* &out) {
    out = qobject_cast<ScriptableError*>(object.toQObject());
}

static QScriptValue queryToScriptValue(QScriptEngine *engine, ScriptableQuery* const &in) {
    return engine->newQObject(in, QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
}

static void queryFromScriptValue(const QScriptValue &object, ScriptableQuery* &out) {
    out = qobject_cast<ScriptableQuery*>(object.toQObject());
}

static QScriptValue recordToScriptValue(QScriptEngine *engine, ScriptableRecord* const &in) {
    return engine->newQObject(in, QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
}

static void recordFromScriptValue(const QScriptValue &object, ScriptableRecord* &out) {
    out = qobject_cast<ScriptableRecord*>(object.toQObject());
}

/*
 * Class
 */

ScriptSql::ScriptSql()
{
}

QStringList ScriptSql::keys() const {
    QStringList result;
    result << QString::fromLatin1("sql");
    return result;
}

void ScriptSql::initialize(const QString &key, QScriptEngine *engine) {
    if (key == QString::fromLatin1("sql")) {
        qScriptRegisterMetaType(engine, databaseToScriptValue, databaseFromScriptValue);
        qScriptRegisterMetaType(engine, errorToScriptValue, errorFromScriptValue);
        qScriptRegisterMetaType(engine, queryToScriptValue, queryFromScriptValue);
        qScriptRegisterMetaType(engine, recordToScriptValue, recordFromScriptValue);

        ScriptableSql *scriptableSql = new ScriptableSql(engine);
        QScriptValue sql = engine->newQObject(scriptableSql, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
        engine->globalObject().setProperty(QString::fromLatin1("sql"), sql, QScriptValue::Undeletable);

    }
}

Q_EXPORT_PLUGIN2(scriptreportsql, ScriptSql)
