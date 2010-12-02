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

#include "scriptreporti18n.h"

#include <QtScript/QScriptEngine>

#include "scriptabletranslator.h"
#include "scriptablei18n.h"

/*
 * Statics
 */

static QScriptValue translatorToScriptValue(QScriptEngine *engine, ScriptableTranslator* const &in) {
    return engine->newQObject(in, QScriptEngine::AutoOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
}

static void translatorFromScriptValue(const QScriptValue &object, ScriptableTranslator* &out) {
    out = qobject_cast<ScriptableTranslator*>(object.toQObject());
}

static QScriptValue localeToScriptValue(QScriptEngine *engine, ScriptableLocale* const &in) {
    return engine->newQObject(in, QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
}

static void localeFromScriptValue(const QScriptValue &object, ScriptableLocale* &out) {
    out = qobject_cast<ScriptableLocale*>(object.toQObject());
}

/*
 * Class
 */

ScriptReportI18N::ScriptReportI18N()
{
}

QStringList ScriptReportI18N::keys() const {
    QStringList result;
    result << QString::fromLatin1("sri18n");
    return result;
}

void ScriptReportI18N::initialize(const QString &key, QScriptEngine *engine) {
    if (key == QString::fromLatin1("sri18n")) {
        qScriptRegisterMetaType(engine, translatorToScriptValue, translatorFromScriptValue);
        qScriptRegisterMetaType(engine, localeToScriptValue, localeFromScriptValue);

        QScriptValue sr = setupPackage(QString::fromLatin1("sr"), engine);

        ScriptableI18N *scriptableI18N = new ScriptableI18N(engine);
        QScriptValue i18n = engine->newQObject(scriptableI18N, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
        sr.setProperty(QString::fromLatin1("i18n"), i18n, QScriptValue::Undeletable);
    }
}

Q_EXPORT_PLUGIN2(scriptreporti18n, ScriptReportI18N)
