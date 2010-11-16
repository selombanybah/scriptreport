#include "shellform.h"
#include "ui_shellform.h"

#include "shellcontroller.h"

#include <QtGui/QMenu>

ShellForm::ShellForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShellForm),
    m_controller(0),
    m_index(-1)
{
    ui->setupUi(this);

    ui->promptContinueLabel->setVisible(false);
    connect(ui->scriptLineEdit, SIGNAL(returnPressed()), SLOT(executeSentence()));

    QMenu *menu = new QMenu(this);
    menu->addAction(ui->globalEngineAction);
    menu->addSeparator();
    menu->addAction(ui->resetAction);
    ui->toolButton->setMenu(menu);

    m_useGlobalEngine = ui->globalEngineAction->isChecked();
    connect(ui->globalEngineAction, SIGNAL(toggled(bool)), SLOT(useGlobalEngine(bool)));
    connect(ui->resetAction, SIGNAL(triggered()), SLOT(reset()));

    TabKeyEventFilter *filter = new TabKeyEventFilter(this);
    ui->scriptLineEdit->installEventFilter(filter);
    connect(filter, SIGNAL(tabPressed()), SLOT(complete()));
    connect(filter, SIGNAL(historyUp()), SLOT(historyUp()));
    connect(filter, SIGNAL(historyDown()), SLOT(historyDown()));

    reset();
}


ShellForm::~ShellForm()
{
    delete ui;
}

void ShellForm::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ShellForm::reset() {
    if (m_controller) {
        m_controller->deleteLater();
    }
    m_history.clear();
    ui->historyPlainTextEdit->clear();

    m_controller = new ShellController(this);
    m_controller->setUseGlobalEngine(m_useGlobalEngine);
    connect(m_controller, SIGNAL(write(QString)), SLOT(write(QString)));
    connect(m_controller, SIGNAL(finished(int)), SLOT(reset()));

    ui->historyPlainTextEdit->appendPlainText(
            tr("Welcome to SRShell, version %1, a javascript shell using Qt Script")
            .arg(m_controller->version()));

    if (m_useGlobalEngine) {
        ui->historyPlainTextEdit->appendPlainText(
                tr("Type \"help()\" or \"sr.engine.help()\" for more information"));
    } else {
        ui->historyPlainTextEdit->appendPlainText(
                tr("Type \"sr.engine.help()\" for more information"));

    }

}

void ShellForm::useGlobalEngine(bool use) {
    m_useGlobalEngine = use;
    reset();
}

void ShellForm::activate() {
    ui->scriptLineEdit->setFocus();
}

bool ShellForm::isActive() const {
    return ui->scriptLineEdit->hasFocus();
}

void ShellForm::executeSentence() {
    QString script = ui->scriptLineEdit->text();

    // enable for autocomplete using ?
//    if (script.endsWith(QChar::fromLatin1('?'))) {
//        script.chop(1);
//        ui->scriptLineEdit->setText(script);
//        complete();
//        return;
//    }

    m_sentence.append(script).append(QChar::fromLatin1('\n'));
    bool isCompleteSentence = m_controller->isCompleteSententence(m_sentence);
    bool isContinue = ui->promptContinueLabel->isVisible();
    if (!isCompleteSentence) {
        QString promp;
        if (isContinue) {
            promp = ui->promptContinueLabel->text();
        } else {
            promp = ui->promptLabel->text();
            ui->promptLabel->setVisible(false);
            ui->promptContinueLabel->setVisible(true);
        }
        ui->historyPlainTextEdit->appendPlainText(QString::fromLatin1("%1 %2").arg(promp).arg(script));
        ui->scriptLineEdit->clear();
    } else {
        QString promp;
        if (isContinue) {
            promp = ui->promptContinueLabel->text();
            ui->promptContinueLabel->setVisible(false);
            ui->promptLabel->setVisible(true);
        } else {
            promp = ui->promptLabel->text();
        }

        ui->historyPlainTextEdit->appendPlainText(QString::fromLatin1("%1 %2").arg(promp).arg(script));
        m_controller->setTextForRead(m_sentence);
        m_controller->runOneSentenceInteractive();
        ui->scriptLineEdit->clear();
        m_sentence.replace(QChar::fromLatin1('\n'), QChar::fromLatin1(' '));
        m_history.append(m_sentence);
        m_index = m_history.length();
        m_sentence.clear();
    }

}

void ShellForm::write(QString text) {
    ui->historyPlainTextEdit->appendPlainText(text);
}

void ShellForm::complete() {
    QString line = ui->scriptLineEdit->text();
    int completitionStartAt = 0;
    QString commonName;
    QStringList completitons = m_controller->completeScriptExpression(line, completitionStartAt, commonName);

    int lenght = completitons.length();
    if (lenght == 1) {
        QString completition = completitons.at(0);
        QString s = line.mid(0, completitionStartAt);
        line = s + completition;
        ui->scriptLineEdit->setText(line);
    } else if (lenght > 1) {
        QStringListIterator it(completitons);
        bool isContinue = ui->promptContinueLabel->isVisible();
        QString promp;
        if (isContinue) {
            promp = ui->promptContinueLabel->text();
        } else {
            promp = ui->promptLabel->text();
        }
        write(QString::fromLatin1("%1 %2").arg(promp).arg(line));
        QString result;
        while (it.hasNext()) {
            result.append(it.next());
            result.append(QString::fromLatin1("    "));
        }
        write(result);
        if (!commonName.isNull()) {
            line = line + commonName;
        }
        ui->scriptLineEdit->setText(line);
    }
}

void ShellForm::historyUp() {
    --m_index;

    if (m_index < 0) {
        ui->scriptLineEdit->clear();
        m_index = -1;
    } else if (m_index >= m_history.length()) {
        ui->scriptLineEdit->clear();
        m_index = m_history.length();
    } else {
        ui->scriptLineEdit->setText(m_history.at(m_index));
    }
}

void ShellForm::historyDown() {
    ++m_index;

    if (m_index < 0) {
        ui->scriptLineEdit->clear();
        m_index = -1;
    } else if (m_index >= m_history.length()) {
        ui->scriptLineEdit->clear();
        m_index = m_history.length();
    } else {
        ui->scriptLineEdit->setText(m_history.at(m_index));
    }
}
