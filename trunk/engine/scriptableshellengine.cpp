#include "scriptableshellengine.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QProcess>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValueIterator>

ScriptableShellEngine::ScriptableShellEngine(Shell *shell, QObject *parent) :
    /*ScriptableEngine(parent),*/
    QObject(parent), QScriptable(),
    m_shell(shell)
{
}

void ScriptableShellEngine::error() {
    QScriptContext *ctx = context();
    int argumentCount = ctx->argumentCount();

    if (argumentCount > 0) {
        for (int i = 0; i < argumentCount - 1; i++) {
            QScriptValue value = ctx->argument(i);
            m_shell->printErr(value, false);
        }
        QScriptValue value = ctx->argument(argumentCount - 1);
        m_shell->printErr(value);
    }
}

void ScriptableShellEngine::print() {
    QScriptContext *ctx = context();
    int argumentCount = ctx->argumentCount();

    if (argumentCount > 0) {
        for (int i = 0; i < argumentCount - 1; i++) {
            QScriptValue value = ctx->argument(i);
            m_shell->printOut(value, false);
        }
        QScriptValue value = ctx->argument(argumentCount - 1);
        m_shell->printOut(value);
    }
}

QString ScriptableShellEngine::read() {
    QScriptContext *ctx = context();
    int argumentCount = ctx->argumentCount();

    if (argumentCount > 0) {
        for (int i = 0; i < argumentCount - 1; i++) {
            QScriptValue value = ctx->argument(i);
            m_shell->printForReadCommand(value, false);
        }
        QScriptValue value = ctx->argument(argumentCount - 1);
        m_shell->printForReadCommand(value);
    }

    return m_shell->readCommand();
}

void ScriptableShellEngine::exit(int returnCode) {
    m_shell->exit(returnCode);
    engine()->abortEvaluation(engine()->undefinedValue());
}

void ScriptableShellEngine::quit() {
    m_shell->exit();
    engine()->abortEvaluation(engine()->undefinedValue());
}

void ScriptableShellEngine::help() {
    m_shell->helpCommand();
}

/*****************************************************************************
 *** From ScriptableEngine ***************************************************
 *****************************************************************************/

QString ScriptableShellEngine::qtVersion() const {
    return QString::fromLatin1(qVersion());
}

QString ScriptableShellEngine::srVersion() const {
    return QString::fromLatin1(APP_VERSION);
}

QStringList ScriptableShellEngine::arguments() const {
    return m_arguments;
}

void ScriptableShellEngine::setArguments(QStringList arguments) {
    m_arguments = arguments;
}

QStringList ScriptableShellEngine::availableExtensions() const {
    return engine()->availableExtensions();
}

QStringList ScriptableShellEngine::importedExtensions() const {
    return engine()->importedExtensions();
}

Q_INVOKABLE void ScriptableShellEngine::importExtension() {
    QScriptContext *ctx = context();
    QScriptEngine *eng = engine();
    int argumentCount = ctx->argumentCount();

    if (argumentCount > 0) {
        for (int i = 0; i < argumentCount; i++) {
            QString name = ctx->argument(i).toString();
            eng->importExtension(name);
            if (eng->hasUncaughtException()) {
                return;
            }
        }
    }
}

QString ScriptableShellEngine::readFile(QString name) {
    QFile scriptFile(name);
    if (!scriptFile.open(QIODevice::ReadOnly)) {
        QString message = QString::fromLatin1("Unable to load the file: %1").arg(name);
        context()->throwError(message);
        return QString();
    }
    QTextStream stream(&scriptFile);
    return stream.readAll();
}

void ScriptableShellEngine::load() {
    QScriptContext *ctx = context();
    QScriptEngine *eng = engine();
    int argumentCount = ctx->argumentCount();

    for (int i = 0; i < argumentCount; i++) {
        QString fileName = ctx->argument(i).toString();
        QFile scriptFile(fileName);
        if (!scriptFile.open(QIODevice::ReadOnly)) {
            QString message = QString::fromLatin1("Unable to load the file: %1").arg(fileName);
            ctx->throwError(message);
            return;
        }
        QTextStream stream(&scriptFile);
        QString contents = stream.readAll();

        eng->evaluate(contents, fileName);
        if (eng->hasUncaughtException()) {
            return;
        }
    }
}

int ScriptableShellEngine::runCommand() {
    QScriptContext *ctx = context();
    int argumentCount = ctx->argumentCount();

    QString commandName;
    QScriptValue sCommandName = ctx->argument(0);
    if (sCommandName.isString()) {
        commandName = sCommandName.toString();
    }
    QStringList arguments;
    for (int i = 1; i < argumentCount -1; i++) {
        arguments << ctx->argument(i).toString();
    }

    QString dir;
    QScriptValue senv;
    QScriptValue sin;
    QScriptValue sout;
    QScriptValue serr;
    QScriptValue sresult;

    QScriptValue last = ctx->argument(argumentCount -1);
    if (!last.isObject()) {
        arguments << last.toString();
    } else {
        dir = last.property(QString::fromLatin1("dir")).toString();
        if(argumentCount == 1) {
            commandName = last.property(QString::fromLatin1("command")).toString();
        }
        arguments << last.property(QString::fromLatin1("args")).toVariant().toStringList();
        senv = last.property(QString::fromLatin1("env"));
        sin = last.property(QString::fromLatin1("input"));
        sout = last.property(QString::fromLatin1("output"));
        serr = last.property(QString::fromLatin1("err"));
        sresult = last.property(QString::fromLatin1("result"));
    }

    if (commandName.isNull()) {
        ctx->throwError(QString::fromLatin1("command not specified"));
        return -3;
    }

    QProcess process;
    if (sout.isValid() || serr.isValid()) {
        process.setProcessChannelMode(QProcess::SeparateChannels);
    } else {
        process.setProcessChannelMode(QProcess::ForwardedChannels);
    }
    process.setWorkingDirectory(dir);
    if (senv.isValid()) {
        QProcessEnvironment enviroment = QProcessEnvironment::systemEnvironment();
        QScriptValueIterator it(senv);
        while (it.hasNext()) {
            it.next();
            enviroment.insert(it.name(), it.value().toString());
        }
        process.setProcessEnvironment(enviroment);
    }

    int returnCode;
    process.start(commandName, arguments);

    if(sin.isValid()) {
        {
            QTextStream stream(&process);
            stream << sin.toString();
        }
        process.closeWriteChannel();
    }

    if (!process.waitForFinished(-1)) {
        if (process.error() == QProcess::FailedToStart) {
            QString message = QString::fromLatin1("%1: command not found").arg(commandName);
            ctx->throwError(message);
        } else if (process.error() == QProcess::FailedToStart) {
            QString message = QString::fromLatin1("%1: the process crashed").arg(commandName);
            ctx->throwError(message);
        } else {
            QString message = QString::fromLatin1("%1: an unknown error occurred").arg(commandName);
            ctx->throwError(message);
        }
        return -2;
    }

    if (process.exitStatus() == QProcess::NormalExit) {
        returnCode = process.exitCode();
    } else {
        QString message = QString::fromLatin1("%1: the process crashed").arg(commandName);
        ctx->throwError(message);
        return -1;
    }

    if (sout.isValid()) {
        process.setReadChannel(QProcess::StandardOutput);
        QTextStream stream(&process);
        QString result = sout.toString() + stream.readAll();
        last.setProperty(QString::fromLatin1("output"), result);
    }

    if (serr.isValid()) {
        process.setReadChannel(QProcess::StandardError);
        QTextStream stream(&process);
        QString result = serr.toString() + stream.readAll();
        last.setProperty(QString::fromLatin1("err"), result);
    }

    if (sresult.isValid()) {
        last.setProperty(QString::fromLatin1("result"), returnCode);
    }

    return returnCode;
}

QString ScriptableShellEngine::getFromEnvironment(QString name) {
    return QProcessEnvironment::systemEnvironment().value(name);
}

Q_INVOKABLE void ScriptableShellEngine::installApplicationTranslatorFunctions(const QScriptValue &object) {
    /*
     * Bug: qsTr doesn't work in global scope
     * Fixed in Qt 4.6.3
     * See: http://bugreports.qt.nokia.com/browse/QTBUG-9775
     */
    engine()->installTranslatorFunctions(object);
}

