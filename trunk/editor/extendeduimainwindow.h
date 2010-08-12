#ifndef EXTENDEDUIMAINWINDOW_H
#define EXTENDEDUIMAINWINDOW_H

#include "ui_mainwindow.h"

#include <QtGui/QActionGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleValidator>
#include <QtGui/QFormLayout>
#include <QtGui/QIntValidator>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>

#include "textdocumentsyntaxhighlighter.h"

/*
 * Class ZoomFactorValidator
 */

class ZoomFactorValidator : public QDoubleValidator
{
public:
    ZoomFactorValidator(QObject* parent)
        : QDoubleValidator(parent) {}
    ZoomFactorValidator(qreal bottom, qreal top, int decimals, QObject *parent)
        : QDoubleValidator(bottom, top, decimals, parent) {}

    State validate(QString &input, int &pos) const
    {
        bool replacePercent = false;
        if (input.endsWith(QLatin1Char('%'))) {
            input = input.left(input.length() - 1);
            replacePercent = true;
        }
        State state = QDoubleValidator::validate(input, pos);
        if (replacePercent)
            input += QLatin1Char('%');
        const int num_size = 4;
        if (state == Intermediate) {
            int i = input.indexOf(QLocale::system().decimalPoint());
            if ((i == -1 && input.size() > num_size)
                || (i != -1 && i > num_size))
                return Invalid;
        }
        return state;
    }
};

/*
 * Class LineEdit
 */

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget* parent = 0)
        : QLineEdit(parent)
    {
        setContextMenuPolicy(Qt::NoContextMenu);
        connect(this, SIGNAL(returnPressed()), SLOT(handleReturnPressed()));
    }

protected:
    void focusInEvent(QFocusEvent *e)
    {
        origText = text();
        QLineEdit::focusInEvent(e);
    }

    void focusOutEvent(QFocusEvent *e)
    {
        if (isModified() && !hasAcceptableInput())
            setText(origText);
        QLineEdit::focusOutEvent(e);
    }

private slots:
    void handleReturnPressed()
    {
        origText = text();
    }

private:
    QString origText;
};

/*
 * Class ControlKeyEventFilter
 */

// Needed for handle Ctrl+1, Ctrl+2 ... Ctrl+9, Ctrl+0 in the sourcEdit
class ControlKeyEventFilter : public QObject
{
    Q_OBJECT

public:
    ControlKeyEventFilter(QObject* parent = 0)
        : QObject(parent)
    {}

protected:
    bool eventFilter(QObject */*obj*/, QEvent *event) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->modifiers() & Qt::ControlModifier) {
                switch(keyEvent->key()) {
                case Qt::Key_1:
                case Qt::Key_2:
                case Qt::Key_3:
                case Qt::Key_4:
                case Qt::Key_5:
                case Qt::Key_6:
                case Qt::Key_7:
                case Qt::Key_8:
                case Qt::Key_9:
                case Qt::Key_0:
                    return true;
                default:
                    return false;
                }
            } else
                return false;
        }
        return false;
    }
};

/*
 * Inline method addShortcut
 */

static inline void addShortcut(QAction *action, const QKeySequence &keySequence) {
    QList<QKeySequence> tmpListKey = action->shortcuts();
    QList<QKeySequence> tmpNewListKey(tmpListKey);
    tmpNewListKey.append(keySequence);
    action->setShortcuts(tmpNewListKey);
}

/*
 * Class ExtendedUiMainWindow
 */

class ExtendedUiMainWindow : public Ui::MainWindow {
public:

    // Staus bar widgets
    QLabel *lineLabel;
    QLabel *columnLabel;

    // widgets, unabe to add with qt designer
    QLineEdit *pageNumEdit;
    QLabel *pageNumLabel;
    QComboBox *zoomFactorComboBox;

    QIntValidator *pageNumEditValidator;

    // actions groups
    QActionGroup *modeActionGroup;
    QActionGroup *navegationGroup;
    QActionGroup *fitGroup;
    QActionGroup *zoomGroup;
    QActionGroup *previewModeGroup;
    QActionGroup *pageLayoutGroup;

    SyntaxHighlighter *printSourceSyntaxHighlighter;
    SyntaxHighlighter *generatedCodeSyntaxHighlighter;
    SyntaxHighlighter *sourceCodeSyntaxHighlighter;

    void setupUi(QMainWindow *MainWindow)
    {
        // Staus bar widgets
        lineLabel = new QLabel(MainWindow);
        columnLabel = new QLabel(MainWindow);

        // Create missing elements
        pageNumEdit = new LineEdit(MainWindow);
        pageNumEdit->setObjectName(QString::fromLatin1("pageNumEdit"));
        pageNumLabel = new QLabel(MainWindow);
        pageNumLabel->setObjectName(QString::fromLatin1("pageNumLabel"));
        zoomFactorComboBox = new QComboBox(MainWindow);
        zoomFactorComboBox->setObjectName(QString::fromLatin1("zoomFactorComboBox"));

        pageNumEditValidator = new QIntValidator(0, 0, MainWindow);
        pageNumEditValidator->setObjectName(QString::fromLatin1("pageNumEditValidator"));

        modeActionGroup = new QActionGroup(MainWindow);
        modeActionGroup->setObjectName(QString::fromLatin1("modeActionGroup"));
        navegationGroup = new QActionGroup(MainWindow);
        navegationGroup->setObjectName(QString::fromLatin1("navegationGroup"));
        fitGroup = new QActionGroup(MainWindow);
        fitGroup->setObjectName(QString::fromLatin1("fitGroup"));
        zoomGroup = new QActionGroup(MainWindow);
        zoomGroup->setObjectName(QString::fromLatin1("zoomGroup"));
        previewModeGroup = new QActionGroup(MainWindow);
        previewModeGroup->setObjectName(QString::fromLatin1("previewModeGroup"));
        pageLayoutGroup = new QActionGroup(MainWindow);
        pageLayoutGroup->setObjectName(QString::fromLatin1("pageLayoutGroup"));

        Ui::MainWindow::setupUi(MainWindow);
        extendedRetranslateUi(MainWindow);

        // Setup the staus bar
        statusBar->addPermanentWidget(lineLabel);
        statusBar->addPermanentWidget(columnLabel);

        addMissingPreviewElements();

        ControlKeyEventFilter *controlKeyEventFilter = new ControlKeyEventFilter(MainWindow);
        sourcePlainTextEdit->installEventFilter(controlKeyEventFilter);

        // Setup standard key (not avaiable in QtDesigner)
        addShortcut(actionNew,       QKeySequence::New);
        addShortcut(actionOpen,      QKeySequence::Open);
        addShortcut(actionSave,      QKeySequence::Save);
        addShortcut(actionSaveAs,    QKeySequence::SaveAs);
        addShortcut(actionClose,     QKeySequence::Close);
        addShortcut(actionExit,      QKeySequence::Quit);
        addShortcut(actionUndo,      QKeySequence::Undo);
        addShortcut(actionRedo,      QKeySequence::Redo);
        addShortcut(actionCut,       QKeySequence::Cut);
        addShortcut(actionCopy,      QKeySequence::Copy);
        addShortcut(actionPaste,     QKeySequence::Paste);
        addShortcut(actionDelete,    QKeySequence::Delete);
        addShortcut(actionSelectAll, QKeySequence::SelectAll);
        addShortcut(actionFind,      QKeySequence::Find);
        addShortcut(actionReplace,   QKeySequence::Replace);
        addShortcut(actionZoomIn,    QKeySequence::ZoomIn);
        addShortcut(actionZoomOut,   QKeySequence::ZoomOut);
        addShortcut(actionFirstPage, QKeySequence::MoveToStartOfDocument);
        addShortcut(actionLastPage,  QKeySequence::MoveToEndOfDocument);
//        addShortcut(actionPreviousPage, QKeySequence::MoveToPreviousPage);
//        addShortcut(actionNextPage,     QKeySequence::MoveToNextPage);
        addShortcut(actionShowPrevious, QKeySequence::Back);
        addShortcut(actionShowNext,     QKeySequence::Forward);
        addShortcut(actionPrint,     QKeySequence::Print);

        // Actions groups

        modeActionGroup->addAction(actionSource);
        modeActionGroup->addAction(actionSourcePreview);
        modeActionGroup->addAction(actionGeneratedCode);
        modeActionGroup->addAction(actionPrintSource);
        modeActionGroup->addAction(actionPrintPreview);
        modeActionGroup->addAction(actionHtmlSubset);
        modeActionGroup->addAction(actionJsFunctions);
        modeActionGroup->setExclusive(true);

        navegationGroup->addAction(actionFirstPage);
        navegationGroup->addAction(actionPreviousPage);
        navegationGroup->addAction(actionPage);
        navegationGroup->addAction(actionNextPage);
        navegationGroup->addAction(actionLastPage);

        fitGroup->addAction(actionFitWidth);
        fitGroup->addAction(actionFitPage);

        zoomGroup->addAction(actionZoom);
        zoomGroup->addAction(actionZoomIn);
        zoomGroup->addAction(actionZoomOut);

        previewModeGroup->addAction(actionShowSinglePage);
        previewModeGroup->addAction(actionShowFacingPages);
        previewModeGroup->addAction(actionShowAllPages);

        pageLayoutGroup->addAction(actionPortrait);
        pageLayoutGroup->addAction(actionLandscape);

        printSourceSyntaxHighlighter = new TextDocumentSyntaxHighlighter(printSourceTextEdit, SyntaxHighlighter::HtmlMode);
        generatedCodeSyntaxHighlighter = new TextDocumentSyntaxHighlighter(generatedCodeTextEdit->document(), SyntaxHighlighter::JavaScriptMode);
        sourceCodeSyntaxHighlighter = new TextDocumentSyntaxHighlighter(sourcePlainTextEdit->document());
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        Ui::MainWindow::retranslateUi(MainWindow);
        extendedRetranslateUi(MainWindow);
    } // retranslateUi

private:
    void extendedRetranslateUi(QMainWindow */*MainWindow*/)
    {
        lineLabel->setText(QApplication::translate("MainWindow", "", 0, QApplication::UnicodeUTF8));
        columnLabel->setText(QApplication::translate("MainWindow", "", 0, QApplication::UnicodeUTF8));
    }

    void addMissingPreviewElements() {
        // Based in the QPrintPreviewDialog's source code

        pageNumEdit->setAlignment(Qt::AlignRight);
        pageNumEdit->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

        pageNumLabel->setText(QLatin1String("/ 0"));
        int cyphersWidth = pageNumEdit->fontMetrics().width(QLatin1String("888"));
        int maxWidth = pageNumEdit->minimumSizeHint().width() + cyphersWidth;
        pageNumEdit->setMinimumWidth(maxWidth);
        pageNumEdit->setMaximumWidth(maxWidth);
        pageNumEdit->setValidator(pageNumEditValidator);
        pageNumEdit->setText(QLatin1String("0"));

        zoomFactorComboBox->setEditable(true);
        zoomFactorComboBox->setMinimumContentsLength(7);
        zoomFactorComboBox->setInsertPolicy(QComboBox::NoInsert);
        LineEdit *zoomEditor = new LineEdit;
        zoomEditor->setValidator(new ZoomFactorValidator(1, 1000, 1, zoomEditor));
        zoomFactorComboBox->setLineEdit(zoomEditor);
        const short factorsX2[] = { 25, 50, 100, 200, 250, 300, 400, 800, 1600 };
        for (int i = 0; i < int(sizeof(factorsX2) / sizeof(factorsX2[0])); ++i)
            zoomFactorComboBox->addItem(QString::fromLatin1("%1%").arg(factorsX2[i] / 2.0));

        zoomFactorComboBox->lineEdit()->setText(QLatin1String("100%"));

        // this is to ensure the label text and the editor text are
        // aligned in all styles - the extra QVBoxLayout is a workaround
        // for bug in QFormLayout
        QWidget *pageEdit = new QWidget(previewToolBar);
        QVBoxLayout *vboxLayout = new QVBoxLayout;
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        QFormLayout *formLayout = new QFormLayout;
        formLayout->setWidget(0, QFormLayout::LabelRole, pageNumEdit);
        formLayout->setWidget(0, QFormLayout::FieldRole, pageNumLabel);
        vboxLayout->addLayout(formLayout);
        vboxLayout->setAlignment(Qt::AlignVCenter);
        pageEdit->setLayout(vboxLayout);

        previewToolBar->insertWidget(actionZoomOut, zoomFactorComboBox);
        previewToolBar->insertWidget(actionNextPage, pageEdit);
    }
};

#endif // EXTENDEDUIMAINWINDOW_H
