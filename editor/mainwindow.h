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
