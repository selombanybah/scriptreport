#include "scriptreportengine.h"

#include <QtCore/QTextStream>
#include <QtGui/QTextDocument>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <QtScript/QScriptEngine>

#include "sourcetransformer.h"
#include "textstreamobject.h"
#include "scriptablereport.h"
#include "scriptableengine.h"

ScriptReportEngine::ScriptReportEngine(QString scriptName, bool isPrintErrorEnabled, bool isWriteWithPrintFunctionTooEnabled) :
        m_isPrintErrorEnabled(isPrintErrorEnabled),
        m_isRunRequired(true),
        m_isUpdateIntermediateCodeRequired(true),
        m_isInitialized(false),
        m_isInEditingMode(false),
        m_isInDebuggingMode(false),
        m_name(scriptName),
        m_isWriteWithPrintFunctionTooEnabled(isWriteWithPrintFunctionTooEnabled),
        m_scriptableReport(0),
        m_scriptableEngine(0)
{
    construct();
}

ScriptReportEngine::ScriptReportEngine(QTextStream *inputStream, QString scriptName, bool isPrintErrorEnabled, bool isWriteWithPrintFunctionTooEnabled) :
        m_isPrintErrorEnabled(isPrintErrorEnabled),
        m_isRunRequired(true),
        m_isUpdateIntermediateCodeRequired(true),
        m_isInitialized(false),
        m_isInEditingMode(false),
        m_isInDebuggingMode(false),
        m_name(scriptName),
        m_isWriteWithPrintFunctionTooEnabled(isWriteWithPrintFunctionTooEnabled),
        m_scriptableReport(0),
        m_scriptableEngine(0)
{
    construct();
    m_inStreamObject->setStream(inputStream);
}

ScriptReportEngine::ScriptReportEngine(QString input, QString scriptName, bool isPrintErrorEnabled, bool isWriteWithPrintFunctionTooEnabled) :
        m_isPrintErrorEnabled(isPrintErrorEnabled),
        m_isRunRequired(true),
        m_isUpdateIntermediateCodeRequired(true),
        m_isInitialized(false),
        m_isInEditingMode(false),
        m_isInDebuggingMode(false),
        m_name(scriptName),
        m_isWriteWithPrintFunctionTooEnabled(isWriteWithPrintFunctionTooEnabled),
        m_scriptableReport(0),
        m_scriptableEngine(0)
{
    construct();
    m_inStreamObject->setText(input);
}

void ScriptReportEngine::construct() {
    m_engine = new QScriptEngine;

    m_inStreamObject = new TextStreamObject(
            QString::fromLatin1("Input"),
            QIODevice::ReadOnly,
            m_engine);

    m_outHeaderStreamObject = new TextStreamObject(
            QString::fromLatin1("Header"),
            QIODevice::WriteOnly,
            m_engine);

    m_outStreamObject = new TextStreamObject(
            QString::fromLatin1("Content"),
            QIODevice::WriteOnly,
            m_engine);

    m_outFooterStreamObject = new TextStreamObject(
            QString::fromLatin1("Footer"),
            QIODevice::WriteOnly,
            m_engine);

    m_printStreamObject = new TextStreamObject(
            QString::fromLatin1("Print"),
            QIODevice::WriteOnly,
            m_engine);
}

ScriptReportEngine::~ScriptReportEngine() {
    delete m_engine;
}

bool ScriptReportEngine::isEditing() const {
    return m_isInEditingMode;
}

bool ScriptReportEngine::isFinal() const {
    return !m_isInEditingMode;
}

void ScriptReportEngine::setEditing(bool editing) {
    m_isInEditingMode = editing;
}

bool ScriptReportEngine::isDebugging() const {
    return m_isInDebuggingMode;
}

void ScriptReportEngine::setDebugging(bool debugging) {
    m_isInDebuggingMode = debugging;
}

bool ScriptReportEngine::isPrintErrorEnabled() const {
    return m_isPrintErrorEnabled;
}

void ScriptReportEngine::setPrintErrorEnabled(bool isPrintErrorEnabled) {
    m_isPrintErrorEnabled = isPrintErrorEnabled;
}

QStringList ScriptReportEngine::arguments() const {
    if (!m_scriptableEngine) {
        return m_scriptableEngine->arguments();
    } else {
        return QStringList();
    }
}

void ScriptReportEngine::setArguments(QStringList arguments) {
    if (!m_scriptableEngine) {
        m_scriptableEngine = new ScriptableEngine(m_engine);
    }
    m_scriptableEngine->setArguments(arguments);
}

QString ScriptReportEngine::scriptName() const {
    return m_name;
}

void ScriptReportEngine::setScriptName(QString scriptName) {
    m_name = scriptName;
}

bool ScriptReportEngine::isWriteWithPrintFunctionTooEnabled() const {
    return m_isWriteWithPrintFunctionTooEnabled;
}

void ScriptReportEngine::setWriteWithPrintFunctionTooEnabled(bool isWriteWithPrintFunctionTooEnabled) {
    m_isWriteWithPrintFunctionTooEnabled = isWriteWithPrintFunctionTooEnabled;
}

TextStreamObject* ScriptReportEngine::input() const {
    return m_inStreamObject;
}

TextStreamObject* ScriptReportEngine::outputHeader() const {
    return m_outHeaderStreamObject;
}

TextStreamObject* ScriptReportEngine::output() const {
    return m_outStreamObject;
}

TextStreamObject* ScriptReportEngine::outputFooter() const {
    return m_outFooterStreamObject;
}

TextStreamObject* ScriptReportEngine::print() const {
    return m_printStreamObject;
}

QScriptEngine* ScriptReportEngine::scriptEngine() /*const*/ {
    if (!m_isInitialized) {
        m_isInitialized = true; // after for prevent an indirect recursive call
        initEngine(*m_engine);
    }
    return m_engine;
}

//void ScriptReportEngine::setScriptEngine(QScriptEngine *scriptEngine) {
//    if (m_engine == scriptEngine) {
//        return;
//    }
//    m_engine = scriptEngine;
//    initEngine(*m_engine);
//}

QString ScriptReportEngine::intermediateCode() const {
    return m_intermediate;
}

QString ScriptReportEngine::errorMessage() const {
    QString message;
    if (m_engine->hasUncaughtException()) {
        QScriptValue exception = m_engine->uncaughtException();
        message = QString::fromLatin1("Uncaught exception: %1. Line: %2")
                  .arg(exception.toString())
                  .arg(m_engine->uncaughtExceptionLineNumber());
        QStringList backtrace = m_engine->uncaughtExceptionBacktrace();
        foreach (QString b, backtrace) {
            message.append(QString::fromLatin1("\n    at %1").arg(b));
        }
    }
    return message;
}

void ScriptReportEngine::initEngine(QScriptEngine &se) {
    if (!m_scriptableReport) {
        QPrinter printer;
        loadPrintConfiguration(printer);
    }
    m_scriptableReport->initEngine(se);

    QScriptValue sr = se.newObject();
    se.globalObject().setProperty(QString::fromLatin1("sr"), sr, QScriptValue::Undeletable);

    QScriptValue report = se.newQObject(m_scriptableReport, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
    sr.setProperty(QString::fromLatin1("report"), report, QScriptValue::Undeletable);

    QScriptValue engine = se.newQObject(m_scriptableEngine, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
    sr.setProperty(QString::fromLatin1("engine"), engine, QScriptValue::Undeletable);

}

void ScriptReportEngine::updateIntermediateCode() {
    m_intermediate.clear();
    QTextStream intermediateStream(&m_intermediate, QIODevice::WriteOnly);

    SourceTransformer st(m_inStreamObject->stream(), &intermediateStream);
    st.transform();
    m_isUpdateIntermediateCodeRequired = false;
    m_isRunRequired = true;
}

void ScriptReportEngine::loadPrintConfiguration(QPrinter &printer) {
    if (!m_scriptableReport) {
        m_scriptableReport = new ScriptableReport(this, m_engine);
    }
    if (!m_scriptableEngine) {
        m_scriptableEngine = new ScriptableEngine(m_engine);
    }
    m_scriptableReport->loadConfigurationFrom(printer);
}

bool ScriptReportEngine::run() {
    if (!m_isInitialized) {
        m_isInitialized = true; // after for prevent an indirect recursive call to scriptEngine()
        initEngine(*m_engine);
    }

    if (m_isUpdateIntermediateCodeRequired) {
        updateIntermediateCode();
    }

    m_engine->evaluate(m_intermediate, m_name);

    m_outHeaderStreamObject->stream()->flush();
    m_outStreamObject->stream()->flush();
    m_outFooterStreamObject->stream()->flush();
    m_printStreamObject->stream()->flush();

    m_isRunRequired = false;
    return m_engine->hasUncaughtException();
}

void ScriptReportEngine::print(QPrinter *printer) {
    // Based on the code published by "Prashant Shah" on October 29, 2008 in the KDE mailing list
    // See: http://lists.kde.org/?l=kde-devel&m=122529598606039&w=2
    // Based on the code of the Qt 4.6 QTextDocument print method

    if (!printer || !printer->isValid()) {
        return;
    }    

    if (m_isRunRequired) {
        run();
    }

    m_scriptableReport->applyConfigurationTo(*printer);
        
    QPainter painter(printer);
    // Check that there is a valid device to print to.
    if (!painter.isActive()) {
        return;
     }

    QRect printerRect(printer->pageRect());

    QString pageName = QString::fromLatin1("##page##");
    QString pageCountName = QString::fromLatin1("##pageCount##");

    QString headerTemplate = m_outHeaderStreamObject->text();
    bool headerHasPage = headerTemplate.contains(pageName);
    bool headerHasPageCount = headerTemplate.contains(pageCountName);
    QString contentTemplate = m_outStreamObject->text();
    bool contentHasPage = contentTemplate.contains(pageName);
    bool contentHasPageCount = contentTemplate.contains(pageCountName);
    QString footerTemplate = m_outFooterStreamObject->text();
    bool footerHasPage = footerTemplate.contains(pageName);
    bool footerHasPageCount = footerTemplate.contains(pageCountName);

    // Setting up the header and calculating the header size
    QTextDocument documentHeader;
    documentHeader.setPageSize(printerRect.size());
    documentHeader.setHtml(headerTemplate);
    QSizeF headerSize = documentHeader.size();

    // Setting up the footer and calculating the footer size
    QTextDocument documentFooter;
    documentFooter.setPageSize(printerRect.size());
    documentFooter.setHtml(footerTemplate);
    QSizeF footerSize = documentFooter.size();

    // Calculating the main document size for one page
    QSizeF centerSize(printerRect.width(),
                      printer->pageRect().height() -
                         headerSize.toSize().height() -
                         footerSize.toSize().height());

    // Setting up the center page
    QTextDocument mainDocument;
    mainDocument.setHtml(contentTemplate);
    mainDocument.setPageSize(centerSize);

    int pageCount = mainDocument.pageCount();
    QString pageCountText = QString::number(pageCount);
    if (headerHasPageCount) {
        headerTemplate.replace(pageCountName, pageCountText);
        documentHeader.setHtml(headerTemplate);
    }
    if (contentHasPageCount) {
        contentTemplate.replace(pageCountName, pageCountText);
        mainDocument.setHtml(contentTemplate);
    }
    if (footerHasPageCount) {
        footerTemplate.replace(pageCountName, pageCountText);
        documentFooter.setHtml(footerTemplate);
    }

    // Setting up the rectangles for each section.
    QRect headerRect  = QRect(QPoint(0,0), documentHeader.size().toSize());
    QRect footerRect  = QRect(QPoint(0,0), documentFooter.size().toSize());

    int docCopies;
    int pageCopies;
    bool collate = printer->collateCopies();
    if (collate){
        docCopies = 1;
        pageCopies = printer->numCopies();
    } else {
        docCopies = printer->numCopies();
        pageCopies = 1;
    }

    int fromPage = printer->fromPage();
    int toPage = printer->toPage();
    bool ascending = true;

    if (fromPage == 0 && toPage == 0) {
        fromPage = 1;
        toPage = pageCount;
    }

    if (toPage < fromPage) {
        // if the user entered a page range outside the actual number
        // of printable pages, just return
        return;
    }

    if (printer->pageOrder() == QPrinter::LastPageFirst) {
        int tmp = fromPage;
        fromPage = toPage;
        toPage = tmp;
        ascending = false;
    }

    for (int i = 0; i < docCopies; i++) {
        // Main content rectangle.
        // Current main content rectangle.
        QRect currentRect = QRect(QPoint(0,0), centerSize.toSize());
        int currentPage = fromPage;

        forever {
            QString page = QString::number(currentPage);
            if (headerHasPage) {
                QString header = QString(headerTemplate).replace(pageName, page);
                documentHeader.setHtml(header);
            }
            if (contentHasPage) {
                QString content = QString(contentTemplate).replace(pageName, page);
                mainDocument.setHtml(content);
            }
            if (footerHasPage) {
                QString footer = QString(footerTemplate).replace(pageName, page);
                documentFooter.setHtml(footer);
            }

            // Move the current rectangle to the area to be printed for the current page
            currentRect.moveTo(0, (currentPage - 1) * currentRect.height());

            for (int j = 0; j < pageCopies; j++) {
                if (printer->printerState() == QPrinter::Aborted
                    || printer->printerState() == QPrinter::Error) {
                    return;
                }

                // Resetting the painter matrix co ordinate system.
                painter.resetMatrix();

                // Applying negative translation of painter co-ordinate system by current main content rectangle top y coordinate.
                painter.translate(0, -currentRect.y());
                // Applying positive translation of painter co-ordinate system by header hight.
                painter.translate(0, headerRect.height());
                // Drawing the center content for current page.
                mainDocument.drawContents(&painter, currentRect);
                ///Resetting the painter matrix co ordinate system.
                painter.resetMatrix();
                // Drawing the header on the top of the page
                documentHeader.drawContents(&painter, headerRect);
                // Applying positive translation of painter co-ordinate system to draw the footer
                painter.translate(0, headerRect.height());
                painter.translate(0, centerSize.height());
                documentFooter.drawContents(&painter, footerRect);

                if (j < pageCopies - 1) {
                    printer->newPage();
                }
            }

            if (currentPage == toPage) {
                if (m_isPrintErrorEnabled) {
                    QString message = errorMessage();
                    if (!message.isNull()) {
                        // Setting up the error and calculating the error size
                        QTextDocument documentError;
                        documentError.setPageSize(printerRect.size());
                        documentError.setPlainText(message);
                        QRect errorRect  = QRect(QPoint(0,0), documentError.size().toSize());

                        printer->newPage();
                        // Resetting the painter matrix co ordinate system.
                        painter.resetMatrix();
                        // Drawing the error on the top of the page
                        documentError.drawContents(&painter, errorRect);
                    }
                }
                break;
            }

            if (ascending) {
                currentPage++;
            } else {
                currentPage--;
            }

            printer->newPage();
        }

        if ( i < docCopies - 1) {
            printer->newPage();
        }
    }

}

