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
