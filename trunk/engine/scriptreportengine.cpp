#include "scriptreportengine.h"

#include <QtCore/QTextStream>
#include <QtGui/QTextDocument>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <QtScript/QScriptEngine>

#include "sourcetransformer.h"
#include "textstreamobject.h"

ScriptReportEngine::ScriptReportEngine()
{
}

ScriptReportEngine::~ScriptReportEngine() {

}

void ScriptReportEngine::initEngine(ScriptReport */*scriptReport*/, QScriptEngine */*engine*/) {

}

void ScriptReportEngine::loadPrintConfiguration(ScriptReport */*scriptReport*/, QPrinter */*printer*/) {

}

void ScriptReportEngine::print(ScriptReport *scriptReport, QPrinter *printer) {
    // Based on the code published by "Prashant Shah" on October 29, 2008 in the KDE mailing list
    // See: http://lists.kde.org/?l=kde-devel&m=122529598606039&w=2
    // Based on the code of the Qt 4.6 QTextDocument print method

    QPainter painter(printer);
    // Check that there is a valid device to print to.
    if (!painter.isActive()) {
        return;
     }

    QRect printerRect(printer->pageRect());

    QString pageName = QString::fromLatin1("##page##");
    QString pageCountName = QString::fromLatin1("##pageCount##");

    QString headerTemplate = scriptReport->outputHeader()->text();
    bool headerHasPage = headerTemplate.contains(pageName);
    bool headerHasPageCount = headerTemplate.contains(pageCountName);
    QString contentTemplate = scriptReport->output()->text();
    bool contentHasPage = contentTemplate.contains(pageName);
    bool contentHasPageCount = contentTemplate.contains(pageCountName);
    QString footerTemplate = scriptReport->outputFooter()->text();
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
                if (scriptReport->isPrintErrorEnabled()) {
                    QString message = scriptReport->errorMessage();
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

