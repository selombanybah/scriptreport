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

#include "scriptreportengine.h"

#include <QtCore/QTextStream>
#include <QtGui/QTextDocument>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <QtScript/QScriptEngine>

#include "sourcetransformer.h"
#include "textstreamobject.h"

class ScriptReportEnginePrivate {
    // Nothing here at this momment
};

/*!
    \class ScriptReportEngine
    \brief Class for render the resultant HTML of a Script Report.

    The ScriptReportEngine class is the engine of Script Report, this class render and print the resultant HTML of
    a report. This class exists for add more backends (not supported at this momment) that \bold QTextDocument.

    \warning Use \l ScriptReport for run a Script Report.
*/

/*!
    \fn ScriptReportEngine::ScriptReportEngine()
     Constructs a Script Report Engine.
*/
ScriptReportEngine::ScriptReportEngine()
    : d(new ScriptReportEnginePrivate())
{
}

/*!
    \fn ScriptReportEngine::~ScriptReportEngine()
    Destroy the Script Report Engine.
*/
ScriptReportEngine::~ScriptReportEngine() {
    delete d;
}

/*!
    \fn void ScriptReportEngine::initEngine(ScriptReport *scriptReport, QScriptEngine *engine)
    \brief Initialize the Script Report Engine.

    Initialize the render backend, the script report taht use this backend is \a scriptReport and
    the engine for run the javascript is \a engine.
*/
void ScriptReportEngine::initEngine(ScriptReport */*scriptReport*/, QScriptEngine */*engine*/) {

}

/*!
    \fn void ScriptReportEngine::loadPrintConfiguration(ScriptReport *scriptReport, QPrinter *printer)
    \brief Load the print configuration from \a printer for the script report \a scriptReport.

    This method is called when the script report load the print configuration.
*/
void ScriptReportEngine::loadPrintConfiguration(ScriptReport */*scriptReport*/, QPrinter */*printer*/) {

}

/*!
    \fn ScriptReportEngine::print(ScriptReport *scriptReport, QPrinter *printer)
    \brief Print the script report \a scriptReport with the \a printer.
*/
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
    QString headerFirstTemplate = scriptReport->outputHeaderFirst()->text();
    QString headerLastTemplate = scriptReport->outputHeaderLast()->text();
    bool headerHasPage = headerTemplate.contains(pageName);
    bool headerHasPageCount = headerTemplate.contains(pageCountName);
    QString contentTemplate = scriptReport->outputContent()->text();
    bool contentHasPage = contentTemplate.contains(pageName);
    bool contentHasPageCount = contentTemplate.contains(pageCountName);
    QString footerTemplate = scriptReport->outputFooter()->text();
    QString footerFirstTemplate = scriptReport->outputFooterFirst()->text();
    QString footerLastTemplate = scriptReport->outputFooterLast()->text();
    bool footerHasPage = footerTemplate.contains(pageName);
    bool footerHasPageCount = footerTemplate.contains(pageCountName);

    if (headerFirstTemplate.isNull()) {
        headerFirstTemplate = headerTemplate;
    }
    if (headerLastTemplate.isNull()) {
        headerLastTemplate = headerTemplate;
    }

    if (footerFirstTemplate.isNull()) {
        footerFirstTemplate = footerTemplate;
    }
    if (footerLastTemplate.isNull()) {
        footerLastTemplate = footerTemplate;
    }

    QTextDocument documentHeader;
    QTextDocument documentFooter;
    QTextDocument mainDocument;

    QMapIterator<QUrl, QPair<int, QVariant> > resourcesIterator(scriptReport->resources());
    while (resourcesIterator.hasNext()) {
         resourcesIterator.next();
         documentHeader.addResource(resourcesIterator.value().first, resourcesIterator.key(), resourcesIterator.value().second);
         documentFooter.addResource(resourcesIterator.value().first, resourcesIterator.key(), resourcesIterator.value().second);
         mainDocument.addResource(resourcesIterator.value().first, resourcesIterator.key(), resourcesIterator.value().second);
     }

    // Setting up the header and calculating the header size
    documentHeader.setPageSize(printerRect.size());
    documentHeader.setHtml(headerTemplate);
    QSizeF headerSize = documentHeader.size();

    // Setting up the footer and calculating the footer size
    documentFooter.setPageSize(printerRect.size());
    documentFooter.setHtml(footerTemplate);
    QSizeF footerSize = documentFooter.size();

    // Calculating the main document size for one page
    QSizeF centerSize(printerRect.width(),
                      printer->pageRect().height() -
                         headerSize.toSize().height() -
                         footerSize.toSize().height());

    // Setting up the center page
    mainDocument.setHtml(contentTemplate);
    mainDocument.setPageSize(centerSize);

    int pageCount = mainDocument.pageCount();
    QString pageCountText = QString::number(pageCount);
    if (headerHasPageCount) {
        headerTemplate.replace(pageCountName, pageCountText);
        documentHeader.setHtml(headerTemplate);
    }
    headerFirstTemplate.replace(pageCountName, pageCountText);
    headerLastTemplate.replace(pageCountName, pageCountText);
    if (contentHasPageCount) {
        contentTemplate.replace(pageCountName, pageCountText);
        mainDocument.setHtml(contentTemplate);
    }
    if (footerHasPageCount) {
        footerTemplate.replace(pageCountName, pageCountText);
        documentFooter.setHtml(footerTemplate);
    }
    footerFirstTemplate.replace(pageCountName, pageCountText);
    footerLastTemplate.replace(pageCountName, pageCountText);

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

    for (int i = 0; i < docCopies; ++i) {
        // Main content rectangle.
        // Current main content rectangle.
        QRect currentRect = QRect(QPoint(0,0), centerSize.toSize());
        int currentPage = fromPage;

        forever {
            QString page = QString::number(currentPage);
            if (currentPage == 1) {
                QString header = QString(headerFirstTemplate).replace(pageName, page);
                documentHeader.setHtml(header);
            } else if (currentPage >= pageCount) {
                QString header = QString(headerLastTemplate).replace(pageName, page);
                documentHeader.setHtml(header);
            } else if (headerHasPage) {
                QString header = QString(headerTemplate).replace(pageName, page);
                documentHeader.setHtml(header);
            } else if (currentPage == 2) {
                documentHeader.setHtml(headerTemplate);
            }

            if (contentHasPage) {
                QString content = QString(contentTemplate).replace(pageName, page);
                mainDocument.setHtml(content);
            }

            if (currentPage == 1) {
                QString footer = QString(footerFirstTemplate).replace(pageName, page);
                documentFooter.setHtml(footer);
            } else if (currentPage >= pageCount) {
                QString footer = QString(footerLastTemplate).replace(pageName, page);
                documentFooter.setHtml(footer);
            } else if (footerHasPage) {
                QString footer = QString(footerTemplate).replace(pageName, page);
                documentFooter.setHtml(footer);
            } else if (currentPage == 2) {
                documentFooter.setHtml(footerTemplate);
            }


            // Move the current rectangle to the area to be printed for the current page
            currentRect.moveTo(0, (currentPage - 1) * currentRect.height());

            for (int j = 0; j < pageCopies; ++j) {
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
                ++currentPage;
            } else {
                --currentPage;
            }

            printer->newPage();
        }

        if ( i < docCopies - 1) {
            printer->newPage();
        }
    }

}

