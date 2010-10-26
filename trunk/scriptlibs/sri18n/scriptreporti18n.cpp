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
