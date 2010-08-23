#include "scriptableengine.h"

#include <QtScript/QScriptEngine>

ScriptableEngine::ScriptableEngine(QObject *parent) :
    QObject(parent), QScriptable()
{
}

QString ScriptableEngine::scriptReportVersion() {
    return QString::fromLatin1(APP_VERSION);
}

QString ScriptableEngine::qtVersion() {
    return QString::fromLatin1(qVersion());
}

QStringList ScriptableEngine::availableExtensions() {
    return engine()->availableExtensions();
}

QStringList ScriptableEngine::importedExtensions() {
    return engine()->importedExtensions();
}

Q_INVOKABLE void ScriptableEngine::importExtension(QString name) {
    engine()->importExtension(name);
}

Q_INVOKABLE void ScriptableEngine::installApplicationTranslatorFunctions(const QScriptValue &object) {
    /*
     * Bug: qsTr doesn't work in global scope
     * Fixed in Qt 4.6.3
     * See: http://bugreports.qt.nokia.com/browse/QTBUG-9775
     */
    engine()->installTranslatorFunctions(object);
}
