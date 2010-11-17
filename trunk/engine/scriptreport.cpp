#include "scriptreport.h"

#include <QtCore/QTextStream>
#include <QtGui/QTextDocument>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <QtScript/QScriptEngine>

#include "sourcetransformer.h"
#include "textstreamobject.h"
#include "scriptable/scriptablereport.h"
#include "scriptable/scriptableengine.h"

#include "scriptreportengine.h"

class ScriptReportPrivate {
public:

    ScriptReportPrivate(QString scriptName) :
            isPrintErrorEnabled(true),
            isRunRequired(true),
            isUpdateIntermediateCodeRequired(true),
            isInitialized(false),
            isInEditingMode(false),
            isInDebuggingMode(false),
            name(scriptName),
            isWriteWithPrintFunctionTooEnabled(false),
            scriptableReport(0),
            scriptableEngine(0),
            scriptReportEngine(0)
    {
        construct();
    }

    void construct() {
        engine = new QScriptEngine;

        inStreamObject = new TextStreamObject(
                QString::fromLatin1("Input"),
                QIODevice::ReadOnly,
                engine);

        outHeaderStreamObject = new TextStreamObject(
                QString::fromLatin1("Header"),
                QIODevice::WriteOnly,
                engine);

        outHeaderFirstStreamObject = new TextStreamObject(
                QString::fromLatin1("HeaderFirst"),
                QIODevice::WriteOnly,
                engine);

        outHeaderLastStreamObject = new TextStreamObject(
                QString::fromLatin1("HeaderLast"),
                QIODevice::WriteOnly,
                engine);

        outStreamObject = new TextStreamObject(
                QString::fromLatin1("Content"),
                QIODevice::WriteOnly,
                engine);

        outFooterStreamObject = new TextStreamObject(
                QString::fromLatin1("Footer"),
                QIODevice::WriteOnly,
                engine);

        outFooterFirstStreamObject = new TextStreamObject(
                QString::fromLatin1("FooterFirst"),
                QIODevice::WriteOnly,
                engine);

        outFooterLastStreamObject = new TextStreamObject(
                QString::fromLatin1("FooterLast"),
                QIODevice::WriteOnly,
                engine);

        printStreamObject = new TextStreamObject(
                QString::fromLatin1("Print"),
                QIODevice::WriteOnly,
                engine);
    }

    void initScriptReportEngine() {
        scriptReportEngine = new ScriptReportEngine();
    }

    bool isPrintErrorEnabled;

    bool isRunRequired;
    bool isUpdateIntermediateCodeRequired;
    bool isInitialized;

    bool isInEditingMode;
    bool isInDebuggingMode;

    QString previousScript;
    QString name;
    QString type;
    bool isWriteWithPrintFunctionTooEnabled;
    QScriptEngine *engine;
    QString intermediate;

    TextStreamObject *inStreamObject;
    TextStreamObject *outHeaderStreamObject;
    TextStreamObject *outHeaderFirstStreamObject;
    TextStreamObject *outHeaderLastStreamObject;
    TextStreamObject *outStreamObject;
    TextStreamObject *outFooterStreamObject;
    TextStreamObject *outFooterFirstStreamObject;
    TextStreamObject *outFooterLastStreamObject;
    TextStreamObject *printStreamObject;

    ScriptableReport *scriptableReport;
    ScriptableEngine *scriptableEngine;

    ScriptReportEngine *scriptReportEngine;
};

ScriptReport::ScriptReport(QString scriptName, QObject *parent) :
        QObject(parent),
        d(new ScriptReportPrivate(scriptName))
{ }

ScriptReport::ScriptReport(QTextStream *inputStream, QString scriptName, QObject *parent) :
        QObject(parent),
        d(new ScriptReportPrivate(scriptName))
{
    d->inStreamObject->setStream(inputStream);
}

ScriptReport::ScriptReport(QString input, QString scriptName, QObject *parent) :
        QObject(parent),
        d(new ScriptReportPrivate(scriptName))
{
    d->inStreamObject->setText(input);
}

ScriptReport::~ScriptReport() {
    delete d->engine;
    delete d->scriptReportEngine;
    delete d;
}

bool ScriptReport::isEditing() const {
    return d->isInEditingMode;
}

void ScriptReport::setEditing(bool editing) {
    d->isInEditingMode = editing;
}

bool ScriptReport::isFinal() const {
    return !d->isInEditingMode;
}

void ScriptReport::setFinal(bool final) {
    d->isInEditingMode = !final;
}

bool ScriptReport::isDebugging() const {
    return d->isInDebuggingMode;
}

void ScriptReport::setDebugging(bool debugging) {
    d->isInDebuggingMode = debugging;
}

bool ScriptReport::isPrintErrorEnabled() const {
    return d->isPrintErrorEnabled;
}

void ScriptReport::setPrintErrorEnabled(bool isPrintErrorEnabled) {
    d->isPrintErrorEnabled = isPrintErrorEnabled;
}

QStringList ScriptReport::arguments() const {
    if (!d->scriptableEngine) {
        return d->scriptableEngine->arguments();
    } else {
        return QStringList();
    }
}

void ScriptReport::setArguments(QStringList arguments) {
    if (!d->scriptableEngine) {
        d->scriptableEngine = new ScriptableEngine(d->engine);
    }
    d->scriptableEngine->setArguments(arguments);
}

QString ScriptReport::previousScript() const {
    return d->previousScript;
}

void ScriptReport::setPreviousScript(QString previousScript) {
    d->previousScript = previousScript;
}

QString ScriptReport::scriptName() const {
    return d->name;
}

void ScriptReport::setScriptName(QString scriptName) {
    d->name = scriptName;
}

bool ScriptReport::isWriteWithPrintFunctionTooEnabled() const {
    return d->isWriteWithPrintFunctionTooEnabled;
}

void ScriptReport::setWriteWithPrintFunctionTooEnabled(bool isWriteWithPrintFunctionTooEnabled) {
    d->isWriteWithPrintFunctionTooEnabled = isWriteWithPrintFunctionTooEnabled;
}

TextStreamObject* ScriptReport::input() const {
    return d->inStreamObject;
}

const TextStreamObject* ScriptReport::outputHeader() const {
    return d->outHeaderStreamObject;
}

const TextStreamObject* ScriptReport::outputHeaderFirst() const {
    return d->outHeaderFirstStreamObject;
}

const TextStreamObject* ScriptReport::outputHeaderLast() const {
    return d->outHeaderLastStreamObject;
}

const TextStreamObject* ScriptReport::output() const {
    return d->outStreamObject;
}

const TextStreamObject* ScriptReport::outputFooter() const {
    return d->outFooterStreamObject;
}

const TextStreamObject* ScriptReport::outputFooterFirst() const {
    return d->outFooterFirstStreamObject;
}

const TextStreamObject* ScriptReport::outputFooterLast() const {
    return d->outFooterLastStreamObject;
}

TextStreamObject* ScriptReport::printOutput() const {
    return d->printStreamObject;
}

QScriptEngine* ScriptReport::scriptEngine() /*const*/ {
    if (!d->isInitialized) {
        d->isInitialized = true; // after for prevent an indirect recursive call
        initEngine();
    }
    return d->engine;
}

//void ScriptReport::setScriptEngine(QScriptEngine *scriptEngine) {
//    if (d->engine == scriptEngine) {
//        return;
//    }
//    d->engine = scriptEngine;
//    initEngine(*d->engine);
//}

ScriptReportEngine* ScriptReport::scriptReportEngine() /*const*/ {
    if (!d->scriptReportEngine) {
        d->initScriptReportEngine();
    }
    return d->scriptReportEngine;
}

QString ScriptReport::intermediateCode() const {
    return d->intermediate;
}

bool ScriptReport::hasUncaughtException() const {
    if (!d->engine) {
        return false;
    }
    return d->engine->hasUncaughtException();
}

QString ScriptReport::errorMessage() const {
    QString message;
    if (!d->engine) {
        return message;
    }
    if (d->engine->hasUncaughtException()) {
        QScriptValue exception = d->engine->uncaughtException();
        message = QString::fromLatin1("Line: %1, Uncaught exception: %2.")
                  .arg(d->engine->uncaughtExceptionLineNumber())
                  .arg(exception.toString());
        QStringList backtrace = d->engine->uncaughtExceptionBacktrace();
        foreach (QString b, backtrace) {
            message.append(QString::fromLatin1("\n    at %1").arg(b));
        }
    }
    return message;
}

void ScriptReport::updateIntermediateCode() {
    d->intermediate.clear();
    QTextStream intermediateStream(&d->intermediate, QIODevice::WriteOnly);

    SourceTransformer st(d->inStreamObject->stream(), &intermediateStream);
    st.transform();
    d->isUpdateIntermediateCodeRequired = false;
    d->isRunRequired = true;
}

void ScriptReport::run() {
    if (!d->isInitialized) {
        d->isInitialized = true; // after for prevent an indirect recursive call to scriptEngine()
        initEngine();
    }

    if (d->isUpdateIntermediateCodeRequired) {
        updateIntermediateCode();
    }

    if (!d->previousScript.isEmpty()) {
        d->engine->evaluate(d->previousScript, QString::fromLatin1("previousScript"));
    }
    d->engine->evaluate(d->intermediate, d->name);

    d->outHeaderStreamObject->stream()->flush();
    d->outStreamObject->stream()->flush();
    d->outFooterStreamObject->stream()->flush();
    d->printStreamObject->stream()->flush();

    d->isRunRequired = false;
}

void ScriptReport::print(QPrinter *printer) {
    if (!printer || !printer->isValid()) {
        return;
    }

    if (d->isRunRequired) {
        run();
    }

    d->scriptableReport->applyConfigurationTo(*printer);
    d->scriptReportEngine->print(this, printer);
}

void ScriptReport::loadPrintConfiguration(QPrinter *printer) {
    if (!printer || !printer->isValid()) {
        return;
    }
    if (!d->scriptableReport) {
        d->scriptableReport = new ScriptableReport(this, d->engine);
    }
    if (!d->scriptableEngine) {
        d->scriptableEngine = new ScriptableEngine(d->engine);
    }
    if (!d->scriptReportEngine) {
        d->initScriptReportEngine();
    }
    d->scriptableReport->loadConfigurationFrom(*printer);
    d->scriptReportEngine->loadPrintConfiguration(this, printer);
}

void ScriptReport::initEngine() {
    if (!d->scriptReportEngine) {
        d->initScriptReportEngine();
    }
    if (!d->scriptableReport) {
        QPrinter printer;
        loadPrintConfiguration(&printer);
    }
    d->scriptableReport->initEngine(*d->engine);

    QScriptValue sr = d->engine->newObject();
    d->engine->globalObject().setProperty(QString::fromLatin1("sr"), sr, QScriptValue::Undeletable);

    QScriptValue report = d->engine->newQObject(d->scriptableReport, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
    sr.setProperty(QString::fromLatin1("report"), report, QScriptValue::Undeletable);

    QScriptValue engine = d->engine->newQObject(d->scriptableEngine, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
    sr.setProperty(QString::fromLatin1("engine"), engine, QScriptValue::Undeletable);

    d->scriptReportEngine->initEngine(this, d->engine);
}
