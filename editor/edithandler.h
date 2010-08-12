#ifndef EDITHANDLER_H
#define EDITHANDLER_H

#include <QObject>

class ExtendedUiMainWindow;
class MainWindow;

class EditHandler : public QObject
{
    Q_OBJECT

public:
    explicit EditHandler(ExtendedUiMainWindow *eui, MainWindow *parent = 0);

    void updateEditActions(bool enabled);
    void updateTextViewActions(bool enabled);
    void startEdit();

public slots:
    void updateLineAndColumn(bool enabled = true);

private slots:
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void delete_();
    void selectAll();
    void find();
    void closeFind();
    void replace();

    void canCut(bool avaiable);
    void canCopy(bool avaiable);
    void canDelete(bool avaiable);
    void canUndo(bool avaiable);
    void canRedo(bool avaiable);
    void clipboardDataChanged();

private:
    void canPaste(bool avaiable);

private:
    ExtendedUiMainWindow *ui;

};

#endif // EDITHANDLER_H
