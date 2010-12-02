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

#ifndef PREVIEWHANDLER_H
#define PREVIEWHANDLER_H

#include <QObject>

class QAction;
class QPrinter;

class ExtendedUiMainWindow;
class MainWindow;

class PreviewHandler : public QObject
{
    Q_OBJECT

public:
    explicit PreviewHandler(ExtendedUiMainWindow *eui, MainWindow *parent = 0);

    void updateZoomActions(bool enabled);
    void updatePrintPreviewActions(bool enabled);
    void updatePrintActions(bool enabled);
    void updatePageLayoutActions(bool enabled);
    void resetEditor();

    QPrinter* printer();

private slots:
    void previewToolBar_pageNumEdited();
    void actionNavegationTriggered(QAction *action);
    void actionFitTriggered(QAction *action);
    void actionPreviewModeTriggered(QAction *action);
    void actionPageLayoutTriggered(QAction *action);
    void zoomIn();
    void zoomOut();
    void zoomFactorChanged();
    void changeCurrentZoom();
    void pageSetup();
    void print();

    void previewChanged();
    void changeCurrentPage();

    void recuperatePreviewPrinter(QPrinter *printer);

private:
    void setPreviewPage(const QString &value);
    void setZoomFactor(const QString &value);

    void updateZoomFactor();

    void updateNavegationActions();
    void updatePreviewModeActions();


private:
    ExtendedUiMainWindow *ui;
    MainWindow *m_parent;
    QPrinter *m_printer;
};

#endif // PREVIEWHANDLER_H
