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

#include "scriptreport.h"

#include <QtCore/QTextStream>
#include <QtCore/QFile>
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

    ScriptReportPrivate(QString reportName = QString()) :
            isPrintErrorEnabled(true),
            isRunRequired(true),
            isUpdateIntermediateCodeRequired(true),
            isInitialized(false),
            isInEditingMode(false),
            isInDebuggingMode(false),
            name(reportName),
            isWriteWithPrintFunctionTooEnabled(false),
            scriptableReport(0),
            scriptableEngine(0),
            scriptReportEngine(0),
            inFile(0),
            lastResourceIndex(0)
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

    QFile *inFile;

    ScriptReportResources resources;
    int lastResourceIndex;
};

/*!
    \class ScriptReport
    \mainclass
    \brief Class for run reports.

    The ScriptReport class allow to run reports created for the Script Report System.

    \tableofcontents

    \section1 Report Specification

    All html tags supported by Qt are supported by the Script Report System, see
    Html Subset tab in the Script Report Editor for
    more information, and for see the Script Report specific tags. See
    JavaScript functions tab in the Script Report Editor for
    more information about JavaScript objects.

    \section1 Runnning a report

    \section2 Run a report
    \code
    QPrinter printer;
    ScriptReport sr("myReport.srt");
    sr.print(&printer);
    \endcode

    \section2 Run a report showing a print dialog
    \code
    QPrinter printer;
    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Accepted) {
        ScriptReport sr("myReport.srt");
        sr.print(&printer);
    }
    \endcode

    \section2 Run a report showing a print preview
    \code
    ScriptReport *sr = new ScriptReport("myReport.srt");
    QPrintPreviewDialog *p = new QPrintPreviewDialog();
    connect(p, SIGNAL(paintRequested(QPrinter*)), sr, SLOT(print(QPrinter*)));
    p->exec();
    delete p;
    delete sr;
    \endcode

*/

/*!
    \fn ScriptReport::ScriptReport(QObject *parent)
    Constructs a Script Report with parent object \a parent.
 */
ScriptReport::ScriptReport(QObject *parent) :
        QObject(parent),
        d(new ScriptReportPrivate())
{
}

/*!
    \fn ScriptReport::ScriptReport(QString reportName, QObject *parent)
    Constructs a Script Report with parent object \a parent and load the report with name \a reportName.
 */
ScriptReport::ScriptReport(QString reportName, QObject *parent) :
        QObject(parent),
        d(new ScriptReportPrivate(reportName))
{
    d->inFile = new QFile(reportName);
    if (d->inFile->open( QIODevice::ReadOnly)) {
        QTextStream *in  = new QTextStream(d->inFile);
        d->inStreamObject->setStream(in);
    } else {
        QString error = QString::fromLatin1("<!--@ throw \"Unable to read the file '%1'\"; -->").arg(reportName);
        d->inStreamObject->setText(error);
    }
}

/*!
    \fn ScriptReport::ScriptReport(QTextStream *inputStream, QString reportName, QObject *parent)
    Constructs a Script Report with parent object \a parent and set the input stream to
    \a inputStream and the report name to \a reportName.
 */
ScriptReport::ScriptReport(QTextStream *inputStream, QString reportName, QObject *parent) :
        QObject(parent),
        d(new ScriptReportPrivate(reportName))
{
    d->inStreamObject->setStream(inputStream);
}

/*!
    \fn ScriptReport::ScriptReport(QString input, QString reportName, QObject *parent)
    Constructs a Script Report with parent object \a parent, read the script from
    \a input and set the report name to \a reportName.
 */
ScriptReport::ScriptReport(QString input, QString reportName, QObject *parent) :
        QObject(parent),
        d(new ScriptReportPrivate(reportName))
{
    d->inStreamObject->setText(input);
}

/*!
    \fn ScriptReport::~ScriptReport()
    Destroy the Script Report.
*/
ScriptReport::~ScriptReport() {
    delete d->engine;
    delete d->scriptReportEngine;
    delete d->inFile;
    delete d;
}

/*!
    \property ScriptReport::isEditing
    \brief Specifies if the report is running in editing mode.

    This property's default is false.
*/
bool ScriptReport::isEditing() const {
    return d->isInEditingMode;
}

void ScriptReport::setEditing(bool editing) {
    d->isInEditingMode = editing;
}

/*!
    \property ScriptReport::isFinal
    \brief Specifies if the report is not running in editing mode.

    This property's default is true.
*/
bool ScriptReport::isFinal() const {
    return !d->isInEditingMode;
}

void ScriptReport::setFinal(bool final) {
    d->isInEditingMode = !final;
}

/*!
    \property ScriptReport::isDebugging
    \brief Specifies if the report is running in debugging mode.

    This property's default is false.
*/
bool ScriptReport::isDebugging() const {
    return d->isInDebuggingMode;
}

void ScriptReport::setDebugging(bool debugging) {
    d->isInDebuggingMode = debugging;
}

/*!
    \property ScriptReport::isPrintErrorEnabled
    \brief Specifies if an error occurs it will be printed.

    This property's default is true.
*/
bool ScriptReport::isPrintErrorEnabled() const {
    return d->isPrintErrorEnabled;
}

void ScriptReport::setPrintErrorEnabled(bool isPrintErrorEnabled) {
    d->isPrintErrorEnabled = isPrintErrorEnabled;
}

/*!
    \property ScriptReport::arguments
    \brief Specifies the script arguments.

    This is avaiable in the script with \c sr.engine.arguments
*/
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


/*!
    \property ScriptReport::previousScript
    \brief Specifies the script that will be run before run the report.
*/
QString ScriptReport::previousScript() const {
    return d->previousScript;
}

void ScriptReport::setPreviousScript(QString previousScript) {
    d->previousScript = previousScript;
}

/*!
    \property ScriptReport::reportName
    \brief Specifies the report file name.
*/
QString ScriptReport::reportName() const {
    return d->name;
}

void ScriptReport::setReportName(QString reportName) {
    d->name = reportName;
}


/*!
    \property ScriptReport::isWriteWithPrintFunctionTooEnabled
    \brief Specifies if use the \c print function to write the sections.
    This is usefull for the debugger.
*/
bool ScriptReport::isWriteWithPrintFunctionTooEnabled() const {
    return d->isWriteWithPrintFunctionTooEnabled;
}

void ScriptReport::setWriteWithPrintFunctionTooEnabled(bool isWriteWithPrintFunctionTooEnabled) {
    d->isWriteWithPrintFunctionTooEnabled = isWriteWithPrintFunctionTooEnabled;
}

/*!
    \fn TextStreamObject* ScriptReport::input() const
    Return the \c TextStreamObject that handle the input stream.
*/
TextStreamObject* ScriptReport::input() const {
    return d->inStreamObject;
}

/*!
    \fn const TextStreamObject* ScriptReport::outputHeader() const
    Return the \c TextStreamObject that handle the output stream of the \c header section.
*/
const TextStreamObject* ScriptReport::outputHeader() const {
    return d->outHeaderStreamObject;
}

/*!
    \fn const TextStreamObject* ScriptReport::outputHeaderFirst() const
    Return the \c TextStreamObject that handle the output stream of the  \c headerFirst section
    (the header of the first page).
*/
const TextStreamObject* ScriptReport::outputHeaderFirst() const {
    return d->outHeaderFirstStreamObject;
}

/*!
    \fn const TextStreamObject* ScriptReport::outputHeaderLast() const
    Return the \c TextStreamObject that handle the output stream of the  \c headerLast section
    (the header of the last page).
*/
const TextStreamObject* ScriptReport::outputHeaderLast() const {
    return d->outHeaderLastStreamObject;
}

/*!
    \fn const TextStreamObject* ScriptReport::outputContent() const
    Return the \c TextStreamObject that handle the output stream of the \c content section.
*/
const TextStreamObject* ScriptReport::outputContent() const {
    return d->outStreamObject;
}

/*!
    \fn const TextStreamObject* ScriptReport::outputFooter() const
    Return the \c TextStreamObject that handle the output stream of the \c footer section.
*/
const TextStreamObject* ScriptReport::outputFooter() const {
    return d->outFooterStreamObject;
}

/*!
    \fn const TextStreamObject* ScriptReport::outputFooterFirst() const
    Return the \c TextStreamObject that handle the output stream of the  \c footerFirst section
    (the footer of the first page).
*/
const TextStreamObject* ScriptReport::outputFooterFirst() const {
    return d->outFooterFirstStreamObject;
}

/*!
    \fn const TextStreamObject* ScriptReport::outputFooterLast() const
    Return the \c TextStreamObject that handle the output stream of the  \c footerFirst section
    (the footer of the last page).
*/
const TextStreamObject* ScriptReport::outputFooterLast() const {
    return d->outFooterLastStreamObject;
}

/*!
    \fn TextStreamObject* ScriptReport::printOutput() const
    Return the \c TextStreamObject that handle the output stream of the \c print function.
*/
TextStreamObject* ScriptReport::printOutput() const {
    return d->printStreamObject;
}

/*!
    \fn QScriptEngine* ScriptReport::scriptEngine()
    Return the script engine of the javascript run enviroment.
*/
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

/*!
    \fn ScriptReportEngine* ScriptReport::scriptReportEngine()
    Return the script report engine use for render the resultant HTML.
*/
ScriptReportEngine* ScriptReport::scriptReportEngine() /*const*/ {
    if (!d->scriptReportEngine) {
        d->initScriptReportEngine();
    }
    return d->scriptReportEngine;
}

/*!
    \property ScriptReport::intermediateCode
    Contains the report transformed to javascript (when it is generated).
    \sa ScriptReport::updateIntermediateCode()
*/
QString ScriptReport::intermediateCode() const {
    return d->intermediate;
}

/*!
    \property ScriptReport::hasUncaughtException
    Specifies if the last script evaluation (report run) resulted in an uncaught exception.
    \sa ScriptReport::errorMessage ScriptReport::run()
*/
bool ScriptReport::hasUncaughtException() const {
    if (!d->engine) {
        return false;
    }
    return d->engine->hasUncaughtException();
}

/*!
    \property ScriptReport::errorMessage
    Contains the last uncaught exception menssage, resulted of the las script evaluation (report run).
    \sa ScriptReport::hasUncaughtException ScriptReport::run()
*/
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

/*!
    \fn void ScriptReport::updateIntermediateCode()
    Transform the report to javascript.
    \sa ScriptReport::intermediateCode
*/
void ScriptReport::updateIntermediateCode() {
    d->intermediate.clear();
    QTextStream intermediateStream(&d->intermediate, QIODevice::WriteOnly);

    SourceTransformer st(d->inStreamObject->stream(), &intermediateStream);
    st.transform();
    d->isUpdateIntermediateCodeRequired = false;
    d->isRunRequired = true;
}

/*!
    \fn void ScriptReport::run()
    \brief Run the intermedial javascript for generate HTML sections code.

    \bold Note: If \c updateIntermediateCode() or \c initEngine() is not runned they will be run.
*/
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

/*!
    \fn void ScriptReport::print(QPrinter *printer)
    \brief Print the report with the \a printer.

    \bold Note: If \c run() is not runned it will be run; if \c run() and \c loadPrintConfiguration(QPrinter*)
    both is not runned they will be runned.
*/
void ScriptReport::print(QPrinter *printer) {
    if (!printer || !printer->isValid()) {
        return;
    }

    if (d->isRunRequired) {
        if (!d->scriptableReport) {
            loadPrintConfiguration(printer);
        }
        run();
    }

    d->scriptableReport->applyConfigurationTo(*printer);
    d->scriptReportEngine->print(this, printer);
}

/*!
    \fn void ScriptReport::loadPrintConfiguration(QPrinter *printer)
    Load the print configuration from \a printer, this configurations will be available in the \c sr.report
    javascript objects.
*/
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

/*!
    \fn void ScriptReport::initEngine()
    Initialize the Qt Script Engine for run the script
*/
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

/*!
    \fn void ScriptReport::addResource(int type, const QVariant & resource, const QUrl & url)

    Adds the resource \a resource to the resource cache, using \a type and \a url as identifiers.
    \a type should be a value from \c QTextDocument::ResourceType.

    For example, you can add an image as a resource in order to reference it from within the document:

    \code
    report->addResource(QTextDocument::ImageResource, QVariant(image), QUrl("mydata://image.png"));
    \endcode

    The image can be inserted into the document using the HTML \c img tag:

    \code
    <img src="mydata://image.png" />
    \endcode

    \sa resources()
*/
void ScriptReport::addResource(int type, const QVariant & resource, const QUrl & url) {
    d->resources.insert(url, ScriptReportResourcePair(type, resource));
}

/*!
    \fn QString ScriptReport::addResource(int type, const QVariant &resource, QString url)
    \overload addResource()

    Adds the resource \a resource to the resource cache, using \a type and \a url as identifiers.
    \a type should be a value from \c QTextDocument::ResourceType. If the \a url is null an url will
    be generated automatically, return the resource's url.

    For example, you can add an image as a resource in order to reference it from within the document:

    \code
    report->addResource(QTextDocument::ImageResource, QVariant(image), "mydata://image.png");
    \endcode

    The image can be inserted into the document using the HTML \c img tag:

    \code
    <img src="mydata://image.png" />
    \endcode

    \sa resources()
*/
QString ScriptReport::addResource(int type, const QVariant &resource, QString url) {
    QString name = url;
    if (name.isNull()) {
        ++d->lastResourceIndex;
        name = QString::fromLatin1("scriptreport://%1").arg(d->lastResourceIndex);
    }
    d->resources.insert(QUrl(name), ScriptReportResourcePair(type,resource));
    return name;
}

/*!
    \typedef ScriptReportResourcePair
    \relates ScriptReport

    Pair of type resource and resource in a \l ScriptReportResources list.

    Synonym for QPair<int, QVariant>.

    \sa ScriptReportResources, ScriptReport::resources()
*/

/*!
    \typedef ScriptReportResources
    \relates ScriptReport

    List of resources in a Script Report.

    Synonym for QMap<QUrl, ScriptReportResourcePair>.

    \sa ScriptReportResourcePair, ScriptReport::resources()
*/

/*!
    \property ScriptReport::resources
    Returns the resource cache.

    \sa addResource()
*/
ScriptReportResources ScriptReport::resources() const {
    return d->resources;
}
