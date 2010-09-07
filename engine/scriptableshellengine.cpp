#include "scriptableshellengine.h"

ScriptableShellEngine::ScriptableShellEngine(Shell *shell, QObject *parent) :
    ScriptableEngine(parent),
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
