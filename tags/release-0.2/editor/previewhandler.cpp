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

#include "previewhandler.h"

#include "extendeduimainwindow.h"
#include "mainwindow.h"

#include <QtGui/QInputDialog>
#include <QtGui/QPageSetupDialog>
#include <QtGui/QPrintDialog>

PreviewHandler::PreviewHandler(ExtendedUiMainWindow *eui, MainWindow *parent) :
    QObject(parent),
    ui(eui),
    m_parent(parent),
    m_printer(0)
{
    connect(ui->pageNumEdit, SIGNAL(editingFinished()), SLOT(previewToolBar_pageNumEdited()));
    connect(ui->zoomFactorComboBox->lineEdit(), SIGNAL(editingFinished()), SLOT(zoomFactorChanged()));
    connect(ui->zoomFactorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(zoomFactorChanged()));
    connect(ui->navegationGroup, SIGNAL(triggered(QAction*)), SLOT(actionNavegationTriggered(QAction*)));

    connect(ui->actionPage, SIGNAL(triggered()), SLOT(changeCurrentPage()));
    connect(ui->fitGroup,   SIGNAL(triggered(QAction*)), SLOT(actionFitTriggered(QAction*)));

    connect(ui->actionZoomIn,  SIGNAL(triggered()), SLOT(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), SLOT(zoomOut()));
    connect(ui->actionZoom,    SIGNAL(triggered()), SLOT(changeCurrentZoom()));

    connect(ui->previewModeGroup, SIGNAL(triggered(QAction*)), SLOT(actionPreviewModeTriggered(QAction*)));
    connect(ui->pageLayoutGroup,  SIGNAL(triggered(QAction*)), SLOT(actionPageLayoutTriggered(QAction*)));
    connect(ui->actionPageSetup,  SIGNAL(triggered()), SLOT(pageSetup()));
    connect(ui->actionPrint,      SIGNAL(triggered()), SLOT(print()));

    connect(ui->printPreviewWidget, SIGNAL(previewChanged()), SLOT(previewChanged()));
    connect(ui->printPreviewWidget, SIGNAL(paintRequested(QPrinter*)), SLOT(recuperatePreviewPrinter(QPrinter*)));
}

void PreviewHandler::resetEditor() {
    if (m_printer) {
        QPrinter p;
        m_printer->setOrientation(p.orientation());
        qreal left, top, right, bottom;
        p.getPageMargins(&left, &top, &right, &bottom, QPrinter::Point);
        m_printer->setPageMargins(left, top, right, bottom, QPrinter::Point);
        QPrinter::PaperSize paperSize = p.paperSize();
        if (paperSize != QPrinter::Custom) {
            m_printer->setPaperSize(paperSize);
        } else {
            QSizeF size = p.paperSize(QPrinter::Point);
            m_printer->setPaperSize(size, QPrinter::Point);
        }
    }
}

QPrinter* PreviewHandler::printer() {
    return m_printer;
}

void PreviewHandler::previewToolBar_pageNumEdited() {
    QString value = ui->pageNumEdit->text();
    setPreviewPage(value);
}

void PreviewHandler::setPreviewPage(const QString &value) {
    if (ui->tabWidget->currentWidget() == ui->printPreviewTab) {
        int page = value.toInt();
        if (page) {
            ui->printPreviewWidget->setCurrentPage(page);
        }
    }
}

void PreviewHandler::actionNavegationTriggered(QAction *action) {
    if (ui->tabWidget->currentWidget() == ui->printPreviewTab) {
        int currentPage = ui->printPreviewWidget->currentPage();
        if (action == ui->actionPreviousPage) {
            ui->printPreviewWidget->setCurrentPage(currentPage - 1);
        } else if (action == ui->actionNextPage) {
            ui->printPreviewWidget->setCurrentPage(currentPage + 1);
        } else if (action == ui->actionFirstPage) {
            ui->printPreviewWidget->setCurrentPage(1);
        } else if (action == ui->actionLastPage) {
            ui->printPreviewWidget->setCurrentPage(ui->printPreviewWidget->pageCount());
        }
    }
    updateNavegationActions();
}

void PreviewHandler::updateNavegationActions() {
    if (ui->tabWidget->currentWidget() == ui->printPreviewTab) {
        int currentPage = ui->printPreviewWidget->currentPage();
        int pageCount = ui->printPreviewWidget->pageCount();
        ui->actionNextPage->setEnabled(currentPage < pageCount);
        ui->actionPreviousPage->setEnabled(currentPage > 1);
        ui->actionFirstPage->setEnabled(currentPage > 1);
        ui->actionLastPage->setEnabled(currentPage < pageCount);
        ui->pageNumEditValidator->setRange(1, pageCount);
        ui->pageNumEdit->setText(QString::number(currentPage));
        ui->pageNumLabel->setText(QString::fromLatin1("/ %1").arg(pageCount));
    }
}

void PreviewHandler::actionFitTriggered(QAction *action) {
    if (ui->tabWidget->currentWidget() == ui->printPreviewTab) {
        if (action == ui->actionFitPage) {
            ui->printPreviewWidget->fitInView();
        } else if (action == ui->actionFitWidth) {
            ui->printPreviewWidget->fitToWidth();
        }
    }
}

void PreviewHandler::actionPreviewModeTriggered(QAction *action) {
    if (ui->tabWidget->currentWidget() == ui->printPreviewTab) {
        if (action == ui->actionShowAllPages) {
            ui->printPreviewWidget->setViewMode(QPrintPreviewWidget::AllPagesView);
        } else if (action == ui->actionShowFacingPages) {
            ui->printPreviewWidget->setViewMode(QPrintPreviewWidget::FacingPagesView);
        } else if (action == ui->actionShowSinglePage){
            ui->printPreviewWidget->setViewMode(QPrintPreviewWidget::SinglePageView);
        }
        updatePreviewModeActions();
    }
}

void PreviewHandler::actionPageLayoutTriggered(QAction *action) {
    if (ui->tabWidget->currentWidget() == ui->printPreviewTab) {
        if (action == ui->actionPortrait) {
            ui->printPreviewWidget->setPortraitOrientation();
        } else if (action == ui->actionLandscape) {
            ui->printPreviewWidget->setLandscapeOrientation();
        }
    }
}

void PreviewHandler::updatePreviewModeActions() {
    if (ui->tabWidget->currentWidget() == ui->printPreviewTab) {
        QPrintPreviewWidget::ViewMode printPreviewViewMode = ui->printPreviewWidget->viewMode();
        switch(printPreviewViewMode) {
        case QPrintPreviewWidget::AllPagesView:
            ui->actionShowAllPages->setChecked(true);
            ui->fitGroup->setEnabled(false);
            ui->navegationGroup->setEnabled(false);
            ui->pageNumEdit->setEnabled(false);
            ui->pageNumLabel->setEnabled(false);
            break;
        case QPrintPreviewWidget::FacingPagesView:
            ui->actionShowFacingPages->setChecked(true);
            ui->fitGroup->setEnabled(true);
            ui->navegationGroup->setEnabled(true);
            ui->pageNumEdit->setEnabled(true);
            ui->pageNumLabel->setEnabled(true);
            break;
        case QPrintPreviewWidget::SinglePageView:
            ui->actionShowSinglePage->setChecked(true);
            ui->fitGroup->setEnabled(true);
            ui->navegationGroup->setEnabled(true);
            ui->pageNumEdit->setEnabled(true);
            ui->pageNumLabel->setEnabled(true);
            break;
        }
    }
}

void PreviewHandler::updatePageLayoutActions(bool enabled) {
    if (enabled) {
        ui->pageLayoutGroup->setEnabled(true);
        if (ui->tabWidget->currentWidget() == ui->printPreviewTab) {
            QPrinter::Orientation orientation = ui->printPreviewWidget->orientation();
            if (orientation == QPrinter::Portrait) {
                ui->actionPortrait->setChecked(true);
            } else if (orientation == QPrinter::Landscape) {
                ui->actionLandscape->setChecked(true);
            }
        }
    } else {
        ui->pageLayoutGroup->setEnabled(false);
        ui->actionPortrait->setChecked(false);
        ui->actionLandscape->setChecked(false);
    }
}

void PreviewHandler::zoomIn() {
    if (ui->tabWidget->currentWidget() == ui->printPreviewTab) {
        ui->printPreviewWidget->zoomIn();
    } else if (ui->tabWidget->currentWidget() == ui->sourcePreviewTab) {
        ui->sourcePreviewTextEdit->zoomIn();
    } else if (ui->tabWidget->currentWidget() == ui->htmlSubsetTab) {
        ui->htmlSubsetTextBrowser->zoomIn();
    } else if (ui->tabWidget->currentWidget() == ui->jsFunctionsTab) {
        ui->jsFunctionsTextBrowser->zoomIn();
    }
    updateZoomFactor();
}

void PreviewHandler::zoomOut() {
    if (ui->tabWidget->currentWidget() == ui->printPreviewTab) {
        ui->printPreviewWidget->zoomOut();
    } else if (ui->tabWidget->currentWidget() == ui->sourcePreviewTab) {
        ui->sourcePreviewTextEdit->zoomOut();
    } else if (ui->tabWidget->currentWidget() == ui->htmlSubsetTab) {
        ui->htmlSubsetTextBrowser->zoomOut();
    } else if (ui->tabWidget->currentWidget() == ui->jsFunctionsTab) {
        ui->jsFunctionsTextBrowser->zoomOut();
    }
    updateZoomFactor();
}

void PreviewHandler::zoomFactorChanged() {
    QString zoom = ui->zoomFactorComboBox->lineEdit()->text();
    setZoomFactor(zoom);
}

void PreviewHandler::setZoomFactor(const QString &value) {
    QString text(value);
    bool ok;
    qreal factor = text.remove(QLatin1Char('%')).toFloat(&ok);
    factor = qMax(qreal(1.0), qMin(qreal(1000.0), factor));
    if (ok) {
        if (ui->tabWidget->currentWidget() == ui->printPreviewTab) {
            ui->printPreviewWidget->setZoomFactor(factor/100.0);
        /*} else if (ui->tabWidget->currentWidget() == ui->sourcePreviewTab) {
            ui->sourcePreviewTextEdit-> TODO
        } else if (ui->tabWidget->currentWidget() == ui->htmlSubsetTab) {
            ui->htmlSubsetTextBrowser-> TODO
        } else if (ui->tabWidget->currentWidget() == ui->jsFunctionsTab) {
            ui->jsFunctionsTextBrowser-> TODO
        */}
        updateZoomFactor();
    }
}

void PreviewHandler::changeCurrentZoom() {
    QString zoomText = ui->zoomFactorComboBox->lineEdit()->text();
    double zoom = zoomText.remove(QLatin1Char('%')).toDouble();

    bool ok;
    double value = QInputDialog::getDouble(m_parent,
                         tr("Zoom"),
                         tr("Select the current zoom (in %):"),
                         zoom,
                         1.0,
                         1000.0,
                         1,
                         &ok);

    if (ok) {
        QString text = QString().sprintf("%.1f%%", value);
        ui->zoomFactorComboBox->lineEdit()->setText(text);
        zoomFactorChanged();
    }
}

void PreviewHandler::pageSetup() {
    QPageSetupDialog pageSetup(m_printer, m_parent);
    if (pageSetup.exec() == QDialog::Accepted) {
        // update possible orientation changes
        if (m_printer->orientation() == QPrinter::Portrait) {
            ui->printPreviewWidget->setPortraitOrientation();
            ui->pageLayoutGroup->blockSignals(true);
            ui->actionPortrait->setChecked(true);
            ui->pageLayoutGroup->blockSignals(false);
        }else {
            ui->printPreviewWidget->setLandscapeOrientation();
            ui->pageLayoutGroup->blockSignals(true);
            ui->actionLandscape->setChecked(true);
            ui->pageLayoutGroup->blockSignals(false);
        }
    }
}

void PreviewHandler::print() {
    QPrintDialog printDialog(m_printer, m_parent);
    if (printDialog.exec() == QDialog::Accepted) {
        ui->printPreviewWidget->print();
    }
}

void PreviewHandler::previewChanged() {
    updateNavegationActions();
    updatePreviewModeActions();
    updateZoomFactor();
}

void PreviewHandler::changeCurrentPage() {
    bool ok;
    int value = QInputDialog::getInt(m_parent,
                         tr("Page"),
                         tr("Select the current page:"),
                         ui->pageNumEdit->text().toInt(),
                         ui->pageNumEditValidator->bottom(),
                         ui->pageNumEditValidator->top(),
                         1,
                         &ok);

    if (ok) {
        ui->pageNumEdit->setText(QString::number(value));
        previewToolBar_pageNumEdited();
    }
}

void PreviewHandler::updateZoomFactor()
{
    qreal zoom;
    if (ui->tabWidget->currentWidget() == ui->printPreviewTab) {
        zoom = ui->printPreviewWidget->zoomFactor() * 100;

        QPrintPreviewWidget::ZoomMode zoomMode = ui->printPreviewWidget->zoomMode();
        switch(zoomMode) {
        case QPrintPreviewWidget::FitToWidth:
            ui->actionFitWidth->setChecked(true);
            break;
        case QPrintPreviewWidget::FitInView:
            ui->actionFitPage->setChecked(true);
            break;
        case QPrintPreviewWidget::CustomZoom:
        default:
            ui->actionFitWidth->setChecked(false);
            ui->actionFitPage->setChecked(false);
            break;
        }

//    } else if (ui->tabWidget->currentWidget() == ui->sourcePreviewTab) {
//        //zoom = TODO
//    } else if (ui->tabWidget->currentWidget() == ui->htmlSubsetTab) {
//        //zoom = TODO
//    } else if (ui->tabWidget->currentWidget() == ui->jsFunctionsTab) {
//        //zoom = TODO
    } else {
        zoom = 100.0;
    }

    if (zoom - int(zoom) == 0.0) {
        ui->zoomFactorComboBox->setEditText(QString::fromLatin1("%1%").arg(zoom));
    } else {
        ui->zoomFactorComboBox->lineEdit()->setText(QString().sprintf("%.1f%%", zoom));
    }
}

void PreviewHandler::updateZoomActions(bool enabled) {
    ui->zoomGroup->setEnabled(enabled);
    ui->zoomFactorComboBox->setEnabled(enabled);
    updateZoomFactor();
}

void PreviewHandler::updatePrintPreviewActions(bool enabled) {
    ui->fitGroup->setEnabled(enabled);
    ui->navegationGroup->setEnabled(enabled);
    ui->pageNumEdit->setEnabled(enabled);
    ui->previewModeGroup->setEnabled(enabled);

    if (!enabled) {
        QAction *checked = ui->fitGroup->checkedAction();
        if (checked) {
            checked->setChecked(false);
        }
        checked = ui->previewModeGroup->checkedAction();
        if (checked) {
            checked->setChecked(false);
        }

        ui->pageNumEditValidator->setRange(0, 0);
        ui->pageNumEdit->setText(QLatin1String("0"));
        ui->pageNumLabel->setText(QLatin1String("/ 0"));
    }
}

void PreviewHandler::updatePrintActions(bool enabled) {
    ui->actionPageSetup->setEnabled(enabled);
    ui->actionPrint->setEnabled(enabled);
}

void PreviewHandler::recuperatePreviewPrinter(QPrinter *printer) {
    if (!m_printer) {
        m_printer = printer;
    }
}
