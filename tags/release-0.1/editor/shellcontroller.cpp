#include "shellcontroller.h"

#include <QtGui/QWidget>
#include <QtGui/QInputDialog>

ShellController::ShellController(QWidget *parent) :
    Shell(parent),
    m_widget(parent)
{
}

QString ShellController::readCommand() {
    QString result = QInputDialog::getText(m_widget, QString::fromLatin1("Shell"), m_buffer);
    m_buffer.clear();
    return result;
}

QString ShellController::readAll(int &/*finalLineNumber*/) {
    return m_readBuffer;
}

QString ShellController::readSentence(int &/*finalLineNumber*/) {
    return m_readBuffer;
}

void ShellController::printForReadCommand(const QScriptValue &message, bool last) {
    m_buffer.append(message.toString());
    if (!last) {
        m_buffer.append(QString::fromLatin1(" "));
    }
}

void ShellController::printOut(const QScriptValue &result, bool last) {
    m_buffer.append(result.toString());
    if (last) {
        emit write(m_buffer);
        m_buffer.clear();
    } else {
        m_buffer.append(QString::fromLatin1(" "));
    }
}

void ShellController::printErr(const QScriptValue &error, bool last) {
    m_buffer.append(error.toString());
    if (last) {
        emit write(m_buffer);
        m_buffer.clear();
    } else {
        m_buffer.append(QString::fromLatin1(" "));
    }
}

void ShellController::printUncaughtException(const QScriptValue &exception) {
    QScriptEngine *eng = engine();
    QString message = QString::fromLatin1("Uncaught exception: %1. Line: %2")
              .arg(exception.toString())
              .arg(eng->uncaughtExceptionLineNumber());
    QStringList backtrace = eng->uncaughtExceptionBacktrace();
    foreach (QString b, backtrace) {
        message.append(QString::fromLatin1("\n    at %1").arg(b));
    }
    emit write(message);
}

void ShellController::helpCommand() {
    emit write(helpMessage());
}

QString ShellController::textForRead() const {
    return m_readBuffer;
}

void ShellController::setTextForRead(QString readText) {
    m_readBuffer = readText;
}
