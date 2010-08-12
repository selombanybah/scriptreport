#include "scriptreportengine.h"

#include <QtCore/QTextStream>
#include <QtGui/QTextDocument>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <QtScript/QScriptEngine>

#include "sourcetransformer.h"
#include "textstreamobject.h"
#include "scriptablereport.h"

ScriptReportEngine::ScriptReportEngine(QString scriptName, bool printResult) :
        m_isUpdateIntermediateCodeRequired(true),
        m_isInitialized(false),
        m_isInEditingMode(false),
        m_isInDebuggingMode(false),
        m_name(scriptName),
        m_isPrintAndWriteEnabled(printResult),
        m_scriptableReport(0)
{
    construct();
}

ScriptReportEngine::ScriptReportEngine(QTextStream *inputStream, QString scriptName, bool printResult) :
        m_isUpdateIntermediateCodeRequired(true),
        m_isInitialized(false),
        m_isInEditingMode(false),
        m_isInDebuggingMode(false),
        m_name(scriptName),
        m_isPrintAndWriteEnabled(printResult),
        m_scriptableReport(0)
{
    construct();
    m_inStreamObject->setStream(inputStream);
}

ScriptReportEngine::ScriptReportEngine(QString input, QString scriptName, bool printResult) :
        m_isUpdateIntermediateCodeRequired(true),
        m_isInitialized(false),
        m_isInEditingMode(false),
        m_isInDebuggingMode(false),
        m_name(scriptName),
        m_isPrintAndWriteEnabled(printResult),
        m_scriptableReport(0)
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
            QString::fromLatin1("Content"),
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

bool ScriptReportEngine::isEditing() {
    return m_isInEditingMode;
}

bool ScriptReportEngine::isFinal() {
    return !m_isInEditingMode;
}

void ScriptReportEngine::setEditing(bool editing) {
    m_isInEditingMode = editing;
}

bool ScriptReportEngine::isDebugging() {
    return m_isInDebuggingMode;
}

void ScriptReportEngine::setDebugging(bool debugging) {
    m_isInDebuggingMode = debugging;
}

QString ScriptReportEngine::scriptName() {
    return m_name;
}

void ScriptReportEngine::setScriptName(QString scriptName) {
    m_name = scriptName;
}

bool ScriptReportEngine::isPrintResultEnabled() {
    return m_isPrintAndWriteEnabled;
}

void ScriptReportEngine::setPrintResultEnabled(bool printResult) {
    m_isPrintAndWriteEnabled = printResult;
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
        initEngine(*m_engine);
        m_isInitialized = true;
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

void ScriptReportEngine::initEngine(QScriptEngine &se) {
    if (!m_scriptableReport) {
        QPrinter printer;
        loadPrintConfiguration(printer);
    }
    m_scriptableReport->initEngine(se);
    QScriptValue report = se.newQObject(m_scriptableReport, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
    se.globalObject().setProperty(QString::fromLatin1("report"), report, QScriptValue::Undeletable);
}

void ScriptReportEngine::updateIntermediateCode() {
    m_intermediate.clear();
    QTextStream intermediateStream(&m_intermediate, QIODevice::WriteOnly);

    SourceTransformer st(m_inStreamObject->stream(), &intermediateStream);
    st.transform();
    m_isUpdateIntermediateCodeRequired = false;
}

void ScriptReportEngine::loadPrintConfiguration(QPrinter &printer) {
    if (!m_scriptableReport) {
        m_scriptableReport = new ScriptableReport(this);
    }
    m_scriptableReport->loadConfigurationFrom(printer);
}

bool ScriptReportEngine::run() {
    if (!m_isInitialized) {
        initEngine(*m_engine);
        m_isInitialized = true;
    }

    if (m_isUpdateIntermediateCodeRequired) {
        updateIntermediateCode();
    }

    m_engine->evaluate(m_intermediate, m_name);

    m_outHeaderStreamObject->stream()->flush();
    m_outStreamObject->stream()->flush();
    m_outFooterStreamObject->stream()->flush();
    m_printStreamObject->stream()->flush();

    return m_engine->hasUncaughtException();
}

void ScriptReportEngine::print(QPrinter *printer) {
    // Based on the code published by "Prashant Shah" on October 29, 2008 in the KDE mailing list
    // See: http://lists.kde.org/?l=kde-devel&m=122529598606039&w=2
    m_scriptableReport->applyConfigurationTo(*printer);

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

    QString pageCount = QString::number(mainDocument.pageCount());
    if (headerHasPageCount) {
        headerTemplate.replace(pageCountName, pageCount);
        documentHeader.setHtml(headerTemplate);
    }
    if (contentHasPageCount) {
        contentTemplate.replace(pageCountName, pageCount);
        mainDocument.setHtml(contentTemplate);
    }
    if (footerHasPageCount) {
        footerTemplate.replace(pageCountName, pageCount);
        documentFooter.setHtml(footerTemplate);
    }

    // Setting up the rectangles for each section.
    QRect headerRect  = QRect(QPoint(0,0), documentHeader.size().toSize());
    QRect footerRect  = QRect(QPoint(0,0), documentFooter.size().toSize());
    QRect contentRect = QRect(QPoint(0,0), mainDocument.size().toSize());

    // Main content rectangle.
    // Current main content rectangle.
    QRect currentRect = QRect(QPoint(0,0), centerSize.toSize());
    int currentPage = 1;
    QPainter painter(printer);
    // Loop if the current content rectangle intersects with the main content rectangle.
    while (currentRect.intersects(contentRect)) {
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

        currentPage++;
        // Translating the current rectangle to the area to be printed for the next page
        currentRect.translate(0, currentRect.height());
        // Inserting a new page if there is till area left to be printed
        if (currentRect.intersects(contentRect)) {
            printer->newPage();
        }
    }
}

