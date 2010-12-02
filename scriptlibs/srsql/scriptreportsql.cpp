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

#include "scriptreportsql.h"

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

ScriptReportSql::ScriptReportSql()
{
}

QStringList ScriptReportSql::keys() const {
    QStringList result;
    result << QString::fromLatin1("srsql");
    return result;
}

void ScriptReportSql::initialize(const QString &key, QScriptEngine *engine) {
    if (key == QString::fromLatin1("srsql")) {
        qScriptRegisterMetaType(engine, databaseToScriptValue, databaseFromScriptValue);
        qScriptRegisterMetaType(engine, errorToScriptValue, errorFromScriptValue);
        qScriptRegisterMetaType(engine, queryToScriptValue, queryFromScriptValue);
        qScriptRegisterMetaType(engine, recordToScriptValue, recordFromScriptValue);

        QScriptValue sr = setupPackage(QString::fromLatin1("sr"), engine);

        ScriptableSql *scriptableSql = new ScriptableSql(engine);
        QScriptValue sql = engine->newQObject(scriptableSql, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
        sr.setProperty(QString::fromLatin1("sql"), sql, QScriptValue::Undeletable);

    }
}

Q_EXPORT_PLUGIN2(scriptreportsql, ScriptReportSql)
