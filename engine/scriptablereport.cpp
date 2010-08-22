#include "scriptablereport.h"

#include <QtCore/QTextStream>
#include <QtGui/QPrinter>
#include <QtScript/QScriptEngine>

#include "scriptreportengine.h"
#include "textstreamobject.h"

/*
 * Statics
 */

static QScriptValue paperToScriptValue(QScriptEngine *engine, ScriptablePaper* const &in) {
    return engine->newQObject(in, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
}

static void paperFromScriptValue(const QScriptValue &object, ScriptablePaper* &out) {
    out = qobject_cast<ScriptablePaper*>(object.toQObject());
}

static QScriptValue print(QScriptContext *context, QScriptEngine *engine) {
    const QChar sp = QChar::fromLatin1(' ');
    const QChar nl = QChar::fromLatin1('\n');

    QScriptValue calleeData = context->callee().data();
    TextStreamObject *outputObject = qobject_cast<TextStreamObject*>(calleeData.toQObject());
    QTextStream *stream = outputObject->stream();
    int argumentCount = context->argumentCount();

    if (argumentCount > 0) {
        QString value = context->argument(0).toString();
        *stream << value;
    }
    for (int i = 1; i < argumentCount; i++) {
        *stream << sp;
        QString value = context->argument(i).toString();
        *stream << value;
    }
    *stream << nl;

    return engine->undefinedValue();
}

/*
 * Class
 */

ScriptableReport::ScriptableReport(ScriptReportEngine *scriptReportEngine, QObject *parent) :
    QObject(parent),
    QScriptable()
{
    m_sre = scriptReportEngine;
    m_scriptablePaper = new ScriptablePaper(this);
    m_title = scriptReportEngine->scriptName();
}

QString ScriptableReport::title() {
    return m_title;
}

void ScriptableReport::setTitle(QString title) {
    m_title = title;
}

QString ScriptableReport::page() {
    return QString::fromLatin1("##page##");
}

QString ScriptableReport::pageCount() {
    return QString::fromLatin1("##pageCount##");
}

QString ScriptableReport::reportEngineVersion() {
    return QString::fromLatin1(APP_VERSION);
}

bool ScriptableReport::isFinal() {
    return m_sre->isFinal();
}

bool ScriptableReport::isEditing() {
    return m_sre->isEditing();
}

bool ScriptableReport::isDebbuging() {
    return m_sre->isDebugging();
}

QStringList ScriptableReport::availableExtensions() {
    return m_sre->scriptEngine()->availableExtensions();
}

QStringList ScriptableReport::importedExtensions() {
    return m_sre->scriptEngine()->importedExtensions();
}

ScriptablePaper* ScriptableReport::paper() {
    return m_scriptablePaper;
}

Q_INVOKABLE void ScriptableReport::writeHeader() {
    if (m_sre->isWriteWithPrintFunctionTooEnabled()) {
        printAndWriteResult(m_sre->outputHeader());
    } else {
        writeResult(m_sre->outputHeader());
    }
}

Q_INVOKABLE void ScriptableReport::writeContent() {
    if (m_sre->isWriteWithPrintFunctionTooEnabled()) {
        printAndWriteResult(m_sre->output());
    } else {
        writeResult(m_sre->output());
    }
}

Q_INVOKABLE void ScriptableReport::writeFooter() {
    if (m_sre->isWriteWithPrintFunctionTooEnabled()) {
        printAndWriteResult(m_sre->outputFooter());
    } else {
        writeResult(m_sre->outputFooter());
    }
}

Q_INVOKABLE void ScriptableReport::importExtension(QString name) {
    m_sre->scriptEngine()->importExtension(name);
}

Q_INVOKABLE void ScriptableReport::installTranslatorFunctions(const QScriptValue & object) {
    /*
     * Bug: qsTr doesn't work in global scope
     * Fixed in Qt 4.6.3
     * See: http://bugreports.qt.nokia.com/browse/QTBUG-9775
     */
    m_sre->scriptEngine()->installTranslatorFunctions();
}

void ScriptableReport::loadConfigurationFrom(QPrinter &printer) {
    QString title = printer.docName();
    if (title.isEmpty()) {
        title = m_sre->scriptName();
    }
    m_title = title;
    m_scriptablePaper->loadConfigurationFrom(printer);
}

void ScriptableReport::applyConfigurationTo(QPrinter &printer) {
    printer.setDocName(m_title);
    m_scriptablePaper->applyConfigurationTo(printer);
}

void ScriptableReport::initEngine(QScriptEngine &engine) {
    qScriptRegisterMetaType(&engine, paperToScriptValue, paperFromScriptValue);

    QScriptValue printFunction = engine.newFunction(print);
    printFunction.setData(engine.newQObject(m_sre->print()));
    engine.globalObject().setProperty(QString::fromLatin1("print"), printFunction, QScriptValue::Undeletable);

    m_scriptablePaper->initEngine(engine);
}

void ScriptableReport::writeResult(TextStreamObject* outputObject)
{
    QTextStream *stream = outputObject->stream();

    for (int i = 0; i < argumentCount(); ++i) {
        QString value = argument(i).toString();
        *stream << value;
    }
}

void ScriptableReport::printAndWriteResult(TextStreamObject* outputObject)
{
    const QString begin1 = QString::fromLatin1(">>> Simple Template Result:");
    const QString begin2 = QString::fromLatin1("Begin >>>");
    const QString end1 = QString::fromLatin1("<<< Simple Template Result:");
    const QString end2 = QString::fromLatin1("End <<<");
    const QString printName = QString::fromLatin1("print");

    QTextStream *stream = outputObject->stream();
    QString name = outputObject->name();

    QScriptValue printFunction = engine()->globalObject().property(printName);

    QScriptValueList beginArgs;
    beginArgs << begin1;
    beginArgs << name;
    beginArgs << begin2;
    printFunction.call(context()->thisObject(), beginArgs);

    printFunction.call(context()->thisObject(), context()->argumentsObject());

    QScriptValueList endArgs;
    endArgs << end1;
    endArgs << name;
    endArgs << end2;
    printFunction.call(context()->thisObject(), endArgs);

    for (int i = 0; i < argumentCount(); ++i) {
        QString value = argument(i).toString();
        *stream << value;
    }
}
