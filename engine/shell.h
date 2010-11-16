#ifndef SHELL_H
#define SHELL_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtScript/QScriptEngine>

#include "scriptreportengine_global.h"

class ScriptableShellEngine;

class SCRIPTREPORTENGINE_EXPORT Shell : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool useGlobalEngine READ useGlobalEngine WRITE setUseGlobalEngine)
    Q_PROPERTY(QStringList arguments READ arguments WRITE setArguments)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
    Q_PROPERTY(int currentLineNumber READ currentLineNumber WRITE setCurrentLineNumber)
    Q_PROPERTY(int processEventsInterval READ processEventsInterval WRITE setProcessEventsInterval)
    Q_PROPERTY(bool isExitCalled READ isExitCalled)
    Q_PROPERTY(bool isEngineInitialized READ isEngineInitialized)
    Q_PROPERTY(int exitCode READ exitCode)
    Q_PROPERTY(QString version READ version)

public:
    explicit Shell(QObject *parent = 0);
    ~Shell();

    virtual QString readCommand() = 0;
    virtual QString readAll(int &finalLineNumber) = 0;
    virtual QString readSentence(int &finalLineNumber) = 0;

    virtual void printForReadCommand(const QScriptValue &message, bool last = true) = 0;
    virtual void printOut(const QScriptValue &result, bool last = true) = 0;
    virtual void printErr(const QScriptValue &error, bool last = true) = 0;

    virtual void printResult(const QScriptValue &result);
    virtual void printUncaughtException(const QScriptValue &exception);

    static bool isCompleteSententence(QString sentence);
    static QString helpMessage();

    bool useGlobalEngine() const;
    void setUseGlobalEngine(bool useGlobalEngine);

    QStringList arguments() const;
    void setArguments(QStringList arguments);

    QString fileName() const;
    void setFileName(QString fileName);

    int currentLineNumber() const;
    void setCurrentLineNumber(int currentLineNumber);

    int processEventsInterval() const;
    void setProcessEventsInterval(int interval);

    bool isExitCalled() const;
    bool isEngineInitialized() const;
    QScriptEngine* engine();

    int exitCode() const;

    QString version() const;

    QStringList completeScriptExpression(QString expression, int &completitionStartAt, QString &commonName);

public slots:
    virtual void helpCommand() = 0;
    void exit(int exitCode = 0);

    void runInteractive();
    void runQuiet();
    void runBatch();
    void runOneSentence();
    void runOneSentenceInteractive();

    void reset();

signals:
    void finished(int exitCode);

protected:
    virtual void initEngine(QScriptEngine &engine);

private:
    int m_lineNumber;
    QScriptEngine *m_engine;
    bool m_isInitialized;
    bool m_useGlobalEngine;
    bool m_exit;
    int m_exitCode;
    QString m_fileName;
    ScriptableShellEngine *m_scriptableEngine;
};

#endif // SHELL_H
