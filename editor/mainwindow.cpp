#include "mainwindow.h"
#include "extendeduimainwindow.h"

#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include <QtCore/QTextStream>

#include <QtScript/QScriptEngine>
#include <QtScriptTools/QScriptEngineDebugger>

#include <ScriptReport/ScriptReportEngine>
#include <ScriptReport/TextStreamObject>
#include <ScriptReport/SourceTransformer>

#include "previewtransformer.h"
#include "edithandler.h"
#include "previewhandler.h"

//include <QPrintPreviewDialog>

const int MainWindow::statusBarTimeout = 2000;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new ExtendedUiMainWindow),
    isValidationRequired(true),
    isSyntacticallyValid(false),
    isRunRequired(true),
    isRunResultValid(false),
    isPrintPreviewUpdateRequired(true),
    scriptReportEngine(0)
{
    applicationPath = QApplication::applicationDirPath();
    QApplication::addLibraryPath(applicationPath);

    ui->setupUi(this);

    editHandler = new EditHandler(ui, this);
    previewHandler = new PreviewHandler(ui, this);

    connect(ui->modeActionGroup, SIGNAL(triggered(QAction*)), SLOT(actionModeTriggered(QAction*)));

    connect(ui->actionNew,          SIGNAL(triggered()), SLOT(new_()));
    connect(ui->actionOpen,         SIGNAL(triggered()), SLOT(open()));
    connect(ui->actionSave,         SIGNAL(triggered()), SLOT(save()));
    connect(ui->actionSaveAs,       SIGNAL(triggered()), SLOT(saveAs()));
    connect(ui->actionClose,        SIGNAL(triggered()), SLOT(closeFile()));
    connect(ui->actionExit,         SIGNAL(triggered()), SLOT(exit()));
    connect(ui->actionShowPrevious, SIGNAL(triggered()), SLOT(showPrevious()));
    connect(ui->actionShowNext,     SIGNAL(triggered()), SLOT(showNext()));
    connect(ui->actionValidate,     SIGNAL(triggered()), SLOT(validate()));
    connect(ui->actionRun,          SIGNAL(triggered()), SLOT(run()));
    connect(ui->actionShowValidationResult, SIGNAL(triggered()), SLOT(showValidationResult()));
    connect(ui->actionShowReportResult,     SIGNAL(triggered()), SLOT(showReportResult()));
    connect(ui->actionShowReportOutput,     SIGNAL(triggered()), SLOT(showReportOutput()));
    connect(ui->actionDebug,        SIGNAL(triggered()), SLOT(debug()));
    connect(ui->actionAbout,        SIGNAL(triggered()), SLOT(about()));
    connect(ui->actionAboutQt,      SIGNAL(triggered()), SLOT(aboutQt()));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(currentTabChanged()));
    connect(ui->sourcePlainTextEdit, SIGNAL(textChanged()), SLOT(sourceModified()));

    connect(ui->printPreviewWidget, SIGNAL(paintRequested(QPrinter*)), SLOT(createPrintPreview(QPrinter*)));

    tabifyDockWidget(ui->validationResultDockWidget, ui->reportResultDockWidget);
    tabifyDockWidget(ui->reportResultDockWidget, ui->reportOutputDockWidget);

    updateNoFile();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (scriptReportEngine) {
        delete scriptReportEngine;
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        if (currentShownName.isEmpty()) {
            setWindowTitle(tr("Script Report Editor"));
        } else {
            setWindowTitle(tr("%1[*] - %2").arg(currentShownName).arg(tr("Script Report Editor")));
        }
        editHandler->updateEditActions(currentShownName.isEmpty());
        break;
    default:
        break;
    }
}

void MainWindow::new_() {
    if (maybeSave()) {
        setCurrentFileName(QString());
        resetEditor();
        startEdit(true);
    }
}

void MainWindow::open() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File..."),
                     QString(), tr("Script report template files (*.srt)"));
        if (!fileName.isEmpty()) {
            load(fileName);
        }
    }
}

bool MainWindow::save() {
    if (currentFileName.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(currentFileName);
    }
}

bool MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this,
                               tr("Save Script Report Template"), QLatin1String("./") + currentShownName,
                               tr("Script report template files (*.srt)"));
    if (fileName.isEmpty())
        return false;

    if (!fileName.endsWith(QLatin1String(".srt"))) {
        fileName += QLatin1String(".srt");
    }

    if (saveFile(fileName)) {
        setCurrentFileName(fileName);
        return true;
    }
    return false;
}

bool MainWindow::saveFile(const QString &fileName) {
    QFile file(fileName);
    bool success = file.open(QIODevice::WriteOnly);
    if (success) {
        QString content = ui->sourcePlainTextEdit->toPlainText();
        QByteArray data = content.toUtf8();
        qint64 c = file.write(data);
        if (c >= data.length()) {
            setWindowModified(false);
            ui->actionSave->setEnabled(false);
            statusBar()->showMessage(tr("File saved"), statusBarTimeout);
            return true;
        } else {
            statusBar()->showMessage(tr("An error has been ocurred writing the data"), statusBarTimeout);
            return false;
        }
    } else {
        statusBar()->showMessage(tr("Unable to open the file to write"), statusBarTimeout);
        return false;
    }
}

void MainWindow::closeFile() {
    if (maybeSave()) {
        updateNoFile();
    }
}

void MainWindow::exit() {
    close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::validate() {
    QString source = ui->sourcePlainTextEdit->toPlainText();
    QTextStream in(&source, QIODevice::ReadOnly);
    QString generatedCode;
    QTextStream out(&generatedCode, QIODevice::WriteOnly);

    SourceTransformer st(&in, &out);
    st.transform();

    validate(generatedCode);
}

bool MainWindow::validate(QString &generatedCode) {
    ui->generatedCodeTextEdit->setPlainText(generatedCode);

    QScriptSyntaxCheckResult result = QScriptEngine::checkSyntax(generatedCode);
    QScriptSyntaxCheckResult::State state = result.state();
    QString message;
    bool isValid = false;
    switch (state) {
    case QScriptSyntaxCheckResult::Error:
        message = tr("Line %1, Column %2: %3.")
                  .arg(QString::number(result.errorLineNumber()),
                       QString::number(result.errorColumnNumber()),
                       result.errorMessage());
        showValidationResult();
        break;
    case QScriptSyntaxCheckResult::Intermediate:
        message = tr("The report is incomplete.");
        showValidationResult();
        break;
    case QScriptSyntaxCheckResult::Valid:
        message = tr("The report is a syntactically correct Qt Script program.");
        isValid = true;
        break;
    default:
        message = QString();
    }

    ui->validationResultTextEdit->setPlainText(message);
    isValidationRequired = false;
    isSyntacticallyValid = isValid;
    return isValid;
}

void MainWindow::run() {
    if (scriptReportEngine) {
        delete scriptReportEngine;
    }
    
    QString source = ui->sourcePlainTextEdit->toPlainText();
    scriptReportEngine = new ScriptReportEngine(source, currentShownName);
    scriptReportEngine->setEditing(true);

    scriptReportEngine->updateIntermediateCode();
    QString generatedCode = scriptReportEngine->intermediateCode();
    ui->generatedCodeTextEdit->setPlainText(generatedCode);

    validate(generatedCode);
//    evit run the report when is not valid
//    if (!validate(generatedCode)) {
//        ui->reportResultTextEdit->setPlainText(QString());
//        ui->printSourceTextEdit->setPlainText(QString());
//        return;
//    }

    QPrinter *printer = previewHandler->printer();
    if (printer) {
        scriptReportEngine->loadPrintConfiguration(*printer);
    }

    scriptReportEngine->run();

    QString printed = scriptReportEngine->print()->text();
    ui->reportOutputTextEdit->setPlainText(printed);
    if (!printed.isEmpty()) {
        showReportOutput();
    }

    QScriptEngine *engine = scriptReportEngine->scriptEngine();
    QString message;
    if (engine->hasUncaughtException()) {
        QScriptValue exception = engine->uncaughtException();
        message = tr("Line: %1, Uncaught exception: %2.")
                  .arg(QString::number(engine->uncaughtExceptionLineNumber()),
                       exception.toString());
        QStringList backtrace = engine->uncaughtExceptionBacktrace();
        foreach (QString b, backtrace) {
            message.append(tr("\n     at %1").arg(b));
        }
        showReportResult();
        isRunResultValid = false;
    } else {
        message = tr("The report was executed successfully.");
        isRunResultValid = true;
    }

    ui->reportResultTextEdit->setPlainText(message);

    QString header = scriptReportEngine->outputHeader()->text();
    QString content = scriptReportEngine->output()->text();
    QString footer = scriptReportEngine->outputFooter()->text();

    QString printSource = QString("<!-- header -->\n%1\n<!-- content -->\n%2\n<!-- footer -->\n%3")
            .arg(header).arg(content).arg(footer);

    ui->printSourceTextEdit->setPlainText(printSource);

    isRunRequired = false;
    isPrintPreviewUpdateRequired = true;
}

void MainWindow::debug() {
    if (scriptReportEngine) {
        // the engine must be deleted for clear all used resources
        delete scriptReportEngine;
        scriptReportEngine = 0;
    }

    QString source = ui->sourcePlainTextEdit->toPlainText();
    QTextStream in(&source, QIODevice::ReadOnly);

    ScriptReportEngine sre(&in, currentShownName, true);
    sre.setEditing(true);
    sre.setDebugging(true);

    QScriptEngineDebugger debugger;
    debugger.attachTo(sre.scriptEngine());
    debugger.action(QScriptEngineDebugger::InterruptAction)->trigger();

    sre.run();
}

void MainWindow::showValidationResult() {
    bool b = ui->reportResultDockWidget->isVisible();
    bool c = ui->reportOutputDockWidget->isVisible();

    ui->validationResultDockWidget->setVisible(false);
    ui->reportResultDockWidget->setVisible(false);
    ui->reportOutputDockWidget->setVisible(false);

    ui->validationResultDockWidget->setVisible(true);

    ui->reportResultDockWidget->setVisible(b);
    ui->reportOutputDockWidget->setVisible(c);
}

void MainWindow::showReportResult() {
    bool a = ui->validationResultDockWidget->isVisible();
    bool c = ui->reportOutputDockWidget->isVisible();

    ui->validationResultDockWidget->setVisible(false);
    ui->reportResultDockWidget->setVisible(false);
    ui->reportOutputDockWidget->setVisible(false);

    ui->reportResultDockWidget->setVisible(true);

    ui->validationResultDockWidget->setVisible(a);
    ui->reportOutputDockWidget->setVisible(c);
}

void MainWindow::showReportOutput() {
    bool a = ui->validationResultDockWidget->isVisible();
    bool b = ui->reportResultDockWidget->isVisible();

    ui->validationResultDockWidget->setVisible(false);
    ui->reportResultDockWidget->setVisible(false);
    ui->reportOutputDockWidget->setVisible(false);

    ui->reportOutputDockWidget->setVisible(true);

    ui->validationResultDockWidget->setVisible(a);
    ui->reportResultDockWidget->setVisible(b);
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Script Report Editor"),
                       tr("<h2>%1 %2</h2>%3")
                       .arg(tr("Script Report Editor"))
                       .arg(QString::fromLatin1(APP_VERSION))
                       .arg(tr("<p>Copyright &copy; 2010 Juan Luis Paz."
                               "<p>Script Report Editor is a small application for "
                               "create reports with HTML and JavaScript using "
                               "Qt classes.")));
}

void MainWindow::aboutQt() {
    QApplication::aboutQt();
}

bool MainWindow::maybeSave() {

    if (!isWindowModified()) {
        return true;
    }

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Script Report Editor"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard
                               | QMessageBox::Cancel);
    if (ret == QMessageBox::Save) {
        return save();
    } else if (ret == QMessageBox::Cancel) {
        return false;
    }
    return true;
}

bool MainWindow::load(const QString &fileName) {
    resetEditor();

    if (!QFile::exists(fileName)) {
        statusBar()->showMessage(tr("File not exist"), statusBarTimeout);
        return false;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        statusBar()->showMessage(tr("Unable to read the file"), statusBarTimeout);
        return false;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString data = in.readAll();
    file.close();

    ui->sourcePlainTextEdit->setPlainText(data);

    setCurrentFileName(fileName);
    startEdit();
    statusBar()->showMessage(tr("File loaded"), statusBarTimeout);
    return true;
}

void MainWindow::setCurrentFileName(const QString &fileName)
{
    currentFileName = fileName;

    QString filePath;
    if (fileName.isEmpty()) {
        currentShownName = tr("untitled");
    } else {
        QFileInfo fileInfo(fileName);
        currentShownName = fileInfo.fileName();
        filePath = fileInfo.absolutePath();
    }

    setWindowTitle(tr("%1[*] - %2").arg(currentShownName).arg(tr("Script Report Editor")));

    if (fileName.startsWith(QLatin1String(":/"))) {
        currentFileName = QString();
        currentFilePath = QString();
    }
    if (!currentFilePath.isEmpty()){
        QApplication::removeLibraryPath(currentFilePath);
    }
    currentFilePath = filePath;
    if (!currentFilePath.isEmpty()){
        QDir::setCurrent(currentFilePath);
        QApplication::addLibraryPath(currentFilePath);
    }
}

void MainWindow::actionModeTriggered(QAction *action) {
    if (action == ui->actionSource) {
        ui->tabWidget->setCurrentWidget(ui->sourceTab);
    } else if (action == ui->actionSourcePreview) {
        ui->tabWidget->setCurrentWidget(ui->sourcePreviewTab);
    } else if (action == ui->actionGeneratedCode) {
        ui->tabWidget->setCurrentWidget(ui->generatedCodeTab);
    } else if (action == ui->actionPrintSource) {
        ui->tabWidget->setCurrentWidget(ui->printSourceTab);
    } else if (action == ui->actionPrintPreview) {
        ui->tabWidget->setCurrentWidget(ui->printPreviewTab);
    } else if (action == ui->actionHtmlSubset) {
        ui->tabWidget->setCurrentWidget(ui->htmlSubsetTab);
    } else if (action == ui->actionJsFunctions) {
        ui->tabWidget->setCurrentWidget(ui->jsFunctionsTab);
    }
}

void MainWindow::currentTabChanged() {
    QWidget *current = ui->tabWidget->currentWidget();
    if (current == ui->sourceTab) {
        ui->modeActionGroup->blockSignals(true);
        ui->actionSource->setChecked(true);
        ui->modeActionGroup->blockSignals(false);
        sourceTabSelected();
    } else if (current == ui->sourcePreviewTab) {
        ui->modeActionGroup->blockSignals(true);
        ui->actionSourcePreview->setChecked(true);
        ui->modeActionGroup->blockSignals(false);
        sourcePreviewTabSelected();
    } else if (current == ui->generatedCodeTab) {
        ui->modeActionGroup->blockSignals(true);
        ui->actionGeneratedCode->setChecked(true);
        ui->modeActionGroup->blockSignals(false);
        generatedCodeTabSelected();
    } else if (current == ui->printSourceTab) {
        ui->modeActionGroup->blockSignals(true);
        ui->actionPrintSource->setChecked(true);
        ui->modeActionGroup->blockSignals(false);
        printSourceTabSelected();
    } else if (current == ui->printPreviewTab) {
        ui->modeActionGroup->blockSignals(true);
        ui->actionPrintPreview->setChecked(true);
        ui->modeActionGroup->blockSignals(false);
        printPreviewTabSelected();
    } else if (current == ui->htmlSubsetTab) {
        ui->modeActionGroup->blockSignals(true);
        ui->actionHtmlSubset->setChecked(true);
        ui->modeActionGroup->blockSignals(false);
        htmlSubsetTabSelected();
    } else if (current == ui->jsFunctionsTab) {
        ui->modeActionGroup->blockSignals(true);
        ui->actionJsFunctions->setChecked(true);
        ui->modeActionGroup->blockSignals(false);
        jsFunctionsTabSelected();
    } else {
        ui->actionSource->setChecked(false);
        ui->actionSourcePreview->setChecked(false);
        ui->actionGeneratedCode->setChecked(false);
        ui->actionPrintPreview->setChecked(false);
        ui->actionHtmlSubset->setChecked(false);

        editHandler->updateEditActions(false);
        editHandler->updateTextViewActions(false);
        previewHandler->updateZoomActions(false);
        previewHandler->updatePrintPreviewActions(false);
    }
}

void MainWindow::showPrevious() {
    int current = ui->tabWidget->currentIndex();
    int max = ui->tabWidget->count();

    int next = (max + current - 1) % max;
    ui->tabWidget->setCurrentIndex(next);
}

void MainWindow::showNext() {
    int current = ui->tabWidget->currentIndex();
    int max = ui->tabWidget->count();

    int next = (current + 1) % max;
    ui->tabWidget->setCurrentIndex(next);
}

void MainWindow::sourceTabSelected(){
    editHandler->updateEditActions(true);
    editHandler->updateTextViewActions(true);
    previewHandler->updateZoomActions(false);
    previewHandler->updatePrintPreviewActions(false);
    previewHandler->updatePageLayoutActions(false);
    previewHandler->updatePrintActions(false);
    editHandler->updateLineAndColumn();
}

void MainWindow::sourcePreviewTabSelected() {
    editHandler->updateEditActions(false);
    editHandler->updateTextViewActions(true);
    previewHandler->updateZoomActions(false);
    //TODO: updateZoomActions(true);
    previewHandler->updatePrintPreviewActions(false);
    previewHandler->updatePageLayoutActions(false);
    previewHandler->updatePrintActions(false);
    QString source = getSourcePreviewCode();
    ui->sourcePreviewTextEdit->setText(source);
    editHandler->updateLineAndColumn();
}

void MainWindow::generatedCodeTabSelected() {
    editHandler->updateEditActions(false);
    editHandler->updateTextViewActions(true);
    previewHandler->updateZoomActions(false);
    previewHandler->updatePrintPreviewActions(false);
    previewHandler->updatePageLayoutActions(false);
    previewHandler->updatePrintActions(false);
    if (isValidationRequired) {
        validate();
    } else if (!isSyntacticallyValid) {
        showValidationResult();
    }
    editHandler->updateLineAndColumn();
}

void MainWindow::printSourceTabSelected() {
    editHandler->updateEditActions(false);
    editHandler->updateTextViewActions(true);
    previewHandler->updateZoomActions(false);
    previewHandler->updatePrintPreviewActions(false);
    previewHandler->updatePageLayoutActions(false);
    previewHandler->updatePrintActions(false);
    if (isRunRequired) {
        run();
    } else if (!isRunResultValid) {
        showReportResult();
    }
    editHandler->updateLineAndColumn();

//    QPrintPreviewDialog *p = new QPrintPreviewDialog;
//    connect(p, SIGNAL(paintRequested(QPrinter*)), SLOT(createPrintPreview(QPrinter*)));
//    p->show();
}

void MainWindow::printPreviewTabSelected() {
    editHandler->updateEditActions(false);
    editHandler->updateTextViewActions(false);
    previewHandler->updateZoomActions(true);
    previewHandler->updatePrintPreviewActions(true);
    previewHandler->updatePageLayoutActions(true);
    previewHandler->updatePrintActions(true);
    if (isPrintPreviewUpdateRequired) {
        if (isRunRequired) {
            run();
        }
        ui->printPreviewWidget->updatePreview();
    } else if (!isRunResultValid) {
        showReportResult();
    }
    editHandler->updateLineAndColumn(false);
}

void MainWindow::htmlSubsetTabSelected() {
    editHandler->updateEditActions(false);
    editHandler->updateTextViewActions(true);
    previewHandler->updateZoomActions(false);
    //TODO: updateZoomActions(true);
    previewHandler->updatePrintPreviewActions(false);
    previewHandler->updatePageLayoutActions(false);
    previewHandler->updatePrintActions(false);
    editHandler->updateLineAndColumn();
}

void MainWindow::jsFunctionsTabSelected() {
    editHandler->updateEditActions(false);
    editHandler->updateTextViewActions(true);
    previewHandler->updateZoomActions(false);
    //TODO: updateZoomActions(true);
    previewHandler->updatePrintPreviewActions(false);
    previewHandler->updatePageLayoutActions(false);
    previewHandler->updatePrintActions(false);
    editHandler->updateLineAndColumn();
}

void MainWindow::updateNoFile() {
    if (!currentFilePath.isEmpty()){
        QApplication::removeLibraryPath(currentFilePath);
        QDir::setCurrent(applicationPath);
    }
    currentFileName.clear();
    currentFilePath.clear();
    currentShownName.clear();
    resetEditor();
    setWindowModified(false);
    setWindowTitle(tr("Script Report Editor"));
    editHandler->updateEditActions(false);
    editHandler->updateTextViewActions(false);
    ui->actionSave->setEnabled(false);
    ui->actionSaveAs->setEnabled(false);
    ui->actionClose->setEnabled(false);
    ui->tabWidget->setVisible(false);

    ui->actionSource->setChecked(false);
    ui->actionSourcePreview->setChecked(false);
    ui->actionGeneratedCode->setChecked(false);
    ui->actionPrintPreview->setChecked(false);
    ui->actionHtmlSubset->setChecked(false);
    ui->actionJsFunctions->setChecked(false);
    ui->modeActionGroup->setEnabled(false);
    ui->actionShowPrevious->setEnabled(false);
    ui->actionShowNext->setEnabled(false);

    ui->actionValidate->setEnabled(false);
    ui->actionRun->setEnabled(false);
    ui->actionShowValidationResult->setEnabled(false);
    ui->actionShowReportResult->setEnabled(false);
    ui->actionShowReportOutput->setEnabled(false);
    ui->actionDebug->setEnabled(false);

    previewHandler->updateZoomActions(false);
    previewHandler->updatePrintPreviewActions(false);
    previewHandler->updatePageLayoutActions(false);
    previewHandler->updatePrintActions(false);

    editHandler->updateLineAndColumn(false);
}

void MainWindow::startEdit(bool canSave) {
    setWindowModified(false);
    ui->tabWidget->setCurrentWidget(ui->sourceTab);
    ui->tabWidget->setVisible(true);
    ui->actionSave->setEnabled(canSave);
    ui->actionSaveAs->setEnabled(true);
    ui->actionClose->setEnabled(true);

    ui->modeActionGroup->setEnabled(true);
    ui->actionShowPrevious->setEnabled(true);
    ui->actionShowNext->setEnabled(true);
    ui->actionSource->setChecked(true); // prevent not actionSource checked when currentTab was sourceTab

    ui->actionValidate->setEnabled(true);
    ui->actionRun->setEnabled(true);
    ui->actionShowValidationResult->setEnabled(true);
    ui->actionShowReportResult->setEnabled(true);
    ui->actionShowReportOutput->setEnabled(true);
    ui->actionDebug->setEnabled(true);

    sourceTabSelected();
    editHandler->startEdit();
}

void MainWindow::resetEditor() {
    ui->sourcePlainTextEdit->clear();
    ui->sourcePreviewTextEdit->clear();
    ui->generatedCodeTextEdit->clear();

    ui->printPreviewWidget->fitInView();
    ui->printPreviewWidget->setCurrentPage(1);

    ui->validationResultDockWidget->setVisible(false);
    ui->reportResultDockWidget->setVisible(false);
    ui->reportOutputDockWidget->setVisible(false);
    ui->validationResultTextEdit->setPlainText(QString());
    ui->reportResultTextEdit->setPlainText(QString());
    ui->reportOutputTextEdit->setPlainText(QString());

    isValidationRequired = true;
    isSyntacticallyValid = false;
    isRunRequired = true;
    isRunResultValid = false;
    isPrintPreviewUpdateRequired = true;

    if (scriptReportEngine) {
        delete scriptReportEngine;
        scriptReportEngine = 0;
    }
    previewHandler->resetEditor();

}

void MainWindow::sourceModified() {    
    if (!currentShownName.isEmpty()) {
        setWindowModified(true);
        ui->actionSave->setEnabled(true);

        isValidationRequired = true;
        isRunRequired = true;
        isPrintPreviewUpdateRequired = true;
    }
}

QString MainWindow::getSourcePreviewCode() {
    QString source = ui->sourcePlainTextEdit->toPlainText();
    QTextStream in(&source, QIODevice::ReadOnly);
    QString code;
    QTextStream out(&code, QIODevice::WriteOnly);

    PreviewTransformer pt(&in, &out);
    pt.transform();

    return code;
}

void MainWindow::createPrintPreview(QPrinter *p) {

    if (scriptReportEngine) {
        scriptReportEngine->print(p);
    }
    isPrintPreviewUpdateRequired = isRunRequired; // prevent set false when the print preview is initializing
}

//include "mainwindow.moc"
