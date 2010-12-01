#include "edithandler.h"

#include "extendeduimainwindow.h"
#include "mainwindow.h"

#include <QtGui/QClipboard>

EditHandler::EditHandler(ExtendedUiMainWindow *eui, MainWindow *parent) :
    QObject(parent),
    ui(eui)
{
    connect(ui->actionUndo,         SIGNAL(triggered()), SLOT(undo()));
    connect(ui->actionRedo,         SIGNAL(triggered()), SLOT(redo()));
    connect(ui->actionCut,          SIGNAL(triggered()), SLOT(cut()));
    connect(ui->actionCopy,         SIGNAL(triggered()), SLOT(copy()));
    connect(ui->actionPaste,        SIGNAL(triggered()), SLOT(paste()));
    connect(ui->actionDelete,       SIGNAL(triggered()), SLOT(delete_()));
    connect(ui->actionSelectAll,    SIGNAL(triggered()), SLOT(selectAll()));
    connect(ui->actionFind,         SIGNAL(triggered()), SLOT(find()));
    connect(ui->actionReplace,      SIGNAL(triggered()), SLOT(replace()));

    connect(ui->sourcePlainTextEdit, SIGNAL(undoAvailable(bool)), SLOT(canUndo(bool)));
    connect(ui->sourcePlainTextEdit, SIGNAL(redoAvailable(bool)), SLOT(canRedo(bool)));
    connect(ui->sourcePlainTextEdit, SIGNAL(copyAvailable(bool)), SLOT(canCut(bool)));
    connect(ui->sourcePlainTextEdit, SIGNAL(copyAvailable(bool)), SLOT(canCopy(bool)));
    connect(ui->sourcePlainTextEdit, SIGNAL(copyAvailable(bool)), SLOT(canDelete(bool)));

    connect(ui->sourcePreviewTextEdit,  SIGNAL(copyAvailable(bool)), SLOT(canCopy(bool)));
    connect(ui->generatedCodeTextEdit,  SIGNAL(copyAvailable(bool)), SLOT(canCopy(bool)));
    connect(ui->printSourceTextEdit,    SIGNAL(copyAvailable(bool)), SLOT(canCopy(bool)));
    connect(ui->htmlSubsetTextBrowser,  SIGNAL(copyAvailable(bool)), SLOT(canCopy(bool)));
    connect(ui->jsFunctionsTextBrowser, SIGNAL(copyAvailable(bool)), SLOT(canCopy(bool)));

    connect(QApplication::clipboard(), SIGNAL(dataChanged()), SLOT(clipboardDataChanged()));

    connect(ui->sourceFindReplaceCloseToolButton, SIGNAL(clicked()), SLOT(closeFind()));

    connect(ui->sourcePlainTextEdit, SIGNAL(cursorPositionChanged()), SLOT(updateLineAndColumn()));
    connect(ui->sourcePreviewTextEdit, SIGNAL(cursorPositionChanged()), SLOT(updateLineAndColumn()));
    connect(ui->generatedCodeTextEdit, SIGNAL(cursorPositionChanged()), SLOT(updateLineAndColumn()));
    connect(ui->printSourceTextEdit, SIGNAL(cursorPositionChanged()), SLOT(updateLineAndColumn()));
    connect(ui->htmlSubsetTextBrowser, SIGNAL(cursorPositionChanged()), SLOT(updateLineAndColumn()));
    connect(ui->jsFunctionsTextBrowser, SIGNAL(cursorPositionChanged()), SLOT(updateLineAndColumn()));

    ui->sourceFindReplaceWidget->setVisible(false);
}

void EditHandler::undo() {
    // Only for source tab
    ui->sourcePlainTextEdit->undo();
}

void EditHandler::redo() {
    // Only for source tab
    ui->sourcePlainTextEdit->redo();
}

void EditHandler::cut() {
    // Only for source tab
    ui->sourcePlainTextEdit->cut();

}

void EditHandler::copy() {
    QWidget *current = ui->tabWidget->currentWidget();
    if (current == ui->sourceTab) {
        ui->sourcePlainTextEdit->copy();
    } else if (current == ui->sourcePreviewTab) {
        ui->sourcePreviewTextEdit->copy();
    } else if (current == ui->generatedCodeTab) {
        ui->generatedCodeTextEdit->copy();
    } else if (current == ui->printSourceTab) {
        ui->printSourceTextEdit->copy();
    } else if (current == ui->htmlSubsetTab) {
        ui->htmlSubsetTextBrowser->copy();
    } else if (current == ui->jsFunctionsTab) {
        ui->jsFunctionsTextBrowser->copy();
    }
}

void EditHandler::paste() {
    // Only for source tab
    ui->sourcePlainTextEdit->paste();
}

void EditHandler::delete_() {
    // Only for source tab
    QTextCursor textCursor = ui->sourcePlainTextEdit->textCursor();
    textCursor.removeSelectedText();
    ui->sourcePlainTextEdit->setTextCursor(textCursor);
}

void EditHandler::selectAll() {
    QWidget *current = ui->tabWidget->currentWidget();
    if (current == ui->sourceTab) {
        ui->sourcePlainTextEdit->selectAll();
    } else if (current == ui->sourcePreviewTab) {
        ui->sourcePreviewTextEdit->selectAll();
    } else if (current == ui->generatedCodeTab) {
        ui->generatedCodeTextEdit->selectAll();
    } else if (current == ui->printSourceTab) {
        ui->printSourceTextEdit->selectAll();
    } else if (current == ui->htmlSubsetTab) {
        ui->htmlSubsetTextBrowser->selectAll();
    } else if (current == ui->jsFunctionsTab) {
        ui->jsFunctionsTextBrowser->selectAll();
    }
}

void EditHandler::find() {
    QWidget *current = ui->tabWidget->currentWidget();
    if (current == ui->sourceTab) {
        ui->sourceFindReplaceWidget->setVisible(true);
        ui->sourceFindLineEdit->setFocus();
    }/* else if (current == ui->sourcePreviewTab) {
    } else if (current == ui->generatedCodeTab) {
    } else if (current == ui->printSourceTab) {
    } else if (current == ui->htmlSubsetTab) {
    } else if (current == ui->jsFunctionsTab) {
    }*/
}

void EditHandler::closeFind() {
    QWidget *current = ui->tabWidget->currentWidget();
    if (current == ui->sourceTab) {
        ui->sourceFindReplaceWidget->setVisible(false);
        ui->sourcePlainTextEdit->setFocus();
    }/* else if (current == ui->sourcePreviewTab) {
    } else if (current == ui->generatedCodeTab) {
    } else if (current == ui->printSourceTab) {
    } else if (current == ui->htmlSubsetTab) {
    } else if (current == ui->jsFunctionsTab) {
    }*/
}

void EditHandler::replace() {
    find();
}

void EditHandler::canCut(bool avaiable) {
    // Only for source tab
    ui->sourcePlainTextEdit->setProperty("sre_canCut", QVariant(avaiable));
    if (ui->tabWidget->currentWidget() == ui->sourceTab) {
        ui->actionCut->setEnabled(avaiable);
    }
}

void EditHandler::canCopy(bool avaiable) {
    QObject *s = sender();
    s->setProperty("sre_canCopy", QVariant(avaiable));

    QWidget *current = ui->tabWidget->currentWidget();
    if (current == ui->sourceTab &&
        s == ui->sourcePlainTextEdit) {
        ui->actionCopy->setEnabled(avaiable);
    } else if (current == ui->sourcePreviewTab &&
               s == ui->sourcePreviewTextEdit) {
        ui->actionCopy->setEnabled(avaiable);
    } else if (current == ui->generatedCodeTab &&
               s == ui->generatedCodeTextEdit) {
        ui->actionCopy->setEnabled(avaiable);
    } else if (current == ui->printSourceTab &&
               s == ui->printSourceTextEdit) {
        ui->actionCopy->setEnabled(avaiable);
    } else if (current == ui->htmlSubsetTab &&
               s == ui->htmlSubsetTextBrowser) {
        ui->actionCopy->setEnabled(avaiable);
    } else if (current == ui->jsFunctionsTab &&
               s == ui->jsFunctionsTextBrowser) {
        ui->actionCopy->setEnabled(avaiable);
    }
}

void EditHandler::canPaste(bool avaiable) {
    // Only for source tab
    ui->sourcePlainTextEdit->setProperty("sre_canPaste", QVariant(avaiable));
    if (ui->tabWidget->currentWidget() == ui->sourceTab) {
        ui->actionPaste->setEnabled(avaiable);
    }
}

void EditHandler::canDelete(bool avaiable) {
    // Only for source tab
    ui->sourcePlainTextEdit->setProperty("sre_canDelete", QVariant(avaiable));
    if (ui->tabWidget->currentWidget() == ui->sourceTab) {
        ui->actionDelete->setEnabled(avaiable);
    }
}

void EditHandler::canUndo(bool avaiable) {
    // Only for source tab
    ui->sourcePlainTextEdit->setProperty("sre_canUndo", QVariant(avaiable));
    if (ui->tabWidget->currentWidget() == ui->sourceTab) {
        ui->actionUndo->setEnabled(avaiable);
    }
}

void EditHandler::canRedo(bool avaiable) {
    // Only for source tab
    ui->sourcePlainTextEdit->setProperty("sre_canRedo", QVariant(avaiable));
    if (ui->tabWidget->currentWidget() == ui->sourceTab) {
        ui->actionRedo->setEnabled(avaiable);
    }
}

void EditHandler::clipboardDataChanged() {
    canPaste(ui->sourcePlainTextEdit->canPaste());
}

void EditHandler::updateEditActions(bool enabled) {
    ui->actionReplace->setEnabled(enabled);

    if (!enabled) {
        ui->actionUndo->setEnabled(false);
        ui->actionRedo->setEnabled(false);
        ui->actionCut->setEnabled(false);
        ui->actionPaste->setEnabled(false);
        ui->actionReplace->setEnabled(false);
        ui->actionDelete->setEnabled(false);
    } else {
        QWidget *current = ui->tabWidget->currentWidget();
        bool sp_canUndo, sp_canRedo, sp_canCut, sp_canPaste, sp_canDelete;
        if (current == ui->sourceTab) {
            sp_canUndo   = ui->sourcePlainTextEdit->property("sre_canUndo").toBool();
            sp_canRedo   = ui->sourcePlainTextEdit->property("sre_canRedo").toBool();
            sp_canCut    = ui->sourcePlainTextEdit->property("sre_canCut").toBool();
            sp_canPaste  = ui->sourcePlainTextEdit->property("sre_canPaste").toBool();
            sp_canDelete = ui->sourcePlainTextEdit->property("sre_canDelete").toBool();
        } else {
            sp_canUndo   = false;
            sp_canRedo   = false;
            sp_canCut    = false;
            sp_canPaste  = false;
            sp_canDelete = false;
        }

        ui->actionUndo->setEnabled(sp_canUndo);
        ui->actionRedo->setEnabled(sp_canRedo);
        ui->actionCut->setEnabled(sp_canCut);
        ui->actionPaste->setEnabled(sp_canPaste);
        ui->actionDelete->setEnabled(sp_canDelete);
    }
}

void EditHandler::updateTextViewActions(bool enabled) {
    ui->actionSelectAll->setEnabled(enabled);
    ui->actionFind->setEnabled(enabled);

    if (!enabled) {
        ui->actionCopy->setEnabled(false);
    } else {
        QWidget *current = ui->tabWidget->currentWidget();
        bool sp_canCopy;
        if (current == ui->sourceTab) {
            sp_canCopy = ui->sourcePlainTextEdit->property("sre_canCopy").toBool();
        } else if (current == ui->sourcePreviewTab) {
            sp_canCopy = ui->sourcePreviewTextEdit->property("sre_canCopy").toBool();
        } else if (current == ui->generatedCodeTab) {
            sp_canCopy = ui->generatedCodeTextEdit->property("sre_canCopy").toBool();
        } else if (current == ui->printSourceTab) {
            sp_canCopy = ui->printSourceTextEdit->property("sre_canCopy").toBool();
        } else if (current == ui->htmlSubsetTab) {
            sp_canCopy = ui->htmlSubsetTextBrowser->property("sre_canCopy").toBool();
        } else if (current == ui->jsFunctionsTab) {
            sp_canCopy = ui->jsFunctionsTextBrowser->property("sre_canCopy").toBool();
        } else {
            sp_canCopy = false;
        }
        ui->actionCopy->setEnabled(sp_canCopy);
    }
}

void EditHandler::startEdit() {
    clipboardDataChanged();
}

void EditHandler::updateLineAndColumn(bool enabled) {
    if (enabled) {
        int line;
        int column;
        QTextCursor textCursor;

        QWidget *current = ui->tabWidget->currentWidget();
        if (current == ui->sourceTab) {
            textCursor = ui->sourcePlainTextEdit->textCursor();
        } else if (current == ui->sourcePreviewTab) {
            textCursor = ui->sourcePreviewTextEdit->textCursor();
        } else if (current == ui->generatedCodeTab) {
            textCursor = ui->generatedCodeTextEdit->textCursor();
        } else if (current == ui->printSourceTab) {
            textCursor = ui->printSourceTextEdit->textCursor();
        } else if (current == ui->htmlSubsetTab) {
            textCursor = ui->htmlSubsetTextBrowser->textCursor();
        } else if (current == ui->jsFunctionsTab) {
            textCursor = ui->jsFunctionsTextBrowser->textCursor();
        }

        if (!textCursor.isNull()) {
            line = textCursor.blockNumber() + 1;
            column = textCursor.columnNumber() + 1;
        } else {
            line = 0;
            column = 0;
        }

        ui->lineLabel->setText(tr("Line: %1").arg(line));
        ui->columnLabel->setText(tr("Column: %1").arg(column));

    } else {
        ui->lineLabel->setText(tr(""));
        ui->columnLabel->setText(tr(""));
    }
}
