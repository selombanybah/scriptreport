#ifndef CONSOLESHELL_H
#define CONSOLESHELL_H

#include <shell.h>

class QFile;
class QTextStream;

class ConsoleShell : public Shell
{
    Q_OBJECT

public:
    explicit ConsoleShell(QObject *parent = 0);
    ~ConsoleShell();

    bool init(int &retunrCode);

    //overrides:
    QString readCommand();
    QString readAll(int &finalLineNumber);
    QString readSentence(int &finalLineNumber);
    void printForReadCommand(const QScriptValue &message, bool last = true);
    void printOut(const QScriptValue &result, bool last = true);
    void printErr(const QScriptValue &error, bool last = true);
    void printUncaughtException(const QScriptValue &exception);
    void helpCommand();

private slots:
    void close(int returnCode);

private:
    void printPrompt();
    void printContinue();
    bool isStdinTty();
    QString readLineTty(bool isContinue);

private:
    enum ShellMode { None, Interactive, Batch, Quiet };
    QFile *m_inFile;
    QTextStream *m_in;
    QTextStream *m_err;
    QTextStream *m_out;
    ShellMode m_mode;
    bool m_isInATty;
    QString m_script;
};

#endif // CONSOLESHELL_H
