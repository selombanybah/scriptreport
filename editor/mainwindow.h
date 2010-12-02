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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class ExtendedUiMainWindow;

class QAction;
class QPrinter;

class ScriptReport;
class EditHandler;
class PreviewHandler;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void new_();
    void open();
    bool save();
    bool saveAs();
    void closeFile();
    void exit();
    void validate();
    void run();
    void showShell();
    void showValidationResult();
    void showReportResult();
    void showReportOutput();
    void debug();
    void about();
    void aboutQt();

    void actionModeTriggered(QAction *action);
    void currentTabChanged();
    void showPrevious();
    void showNext();
    void sourceModified();

    void createPrintPreview(QPrinter *printer);

private:
    bool validate(QString &generatedCode);

    void sourceTabSelected();
    void sourcePreviewTabSelected();
    void generatedCodeTabSelected();
    void printSourceTabSelected();
    void printPreviewTabSelected();
    void htmlSubsetTabSelected();
    void jsFunctionsTabSelected();

    bool maybeSave();
    bool saveFile(const QString &fileName);
    bool load(const QString &fileName);
    void setCurrentFileName(const QString &fileName);
    void startEdit(bool canSave = false);
    void resetEditor();

    void updateNoFile();

    QString getSourcePreviewCode();

private:
    ExtendedUiMainWindow *ui;
    QString applicationPath;
    QString currentFilePath;
    QString currentFileName;
    QString currentShownName;

    bool isValidationRequired;
    bool isSyntacticallyValid;
    bool isRunRequired;
    bool isRunResultValid;
    bool isPrintPreviewUpdateRequired;

    ScriptReport *scriptReport;

    EditHandler *editHandler;
    PreviewHandler *previewHandler;

    static const int statusBarTimeout;
};

#endif // MAINWINDOW_H
