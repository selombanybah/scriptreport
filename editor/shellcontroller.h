#ifndef SHELLCONTROLLER_H
#define SHELLCONTROLLER_H

#include <ScriptReport/Shell>

class QWidget;

class ShellController : public Shell
{
    Q_OBJECT
    Q_PROPERTY (QString textForRead READ textForRead WRITE setTextForRead)

public:
    explicit ShellController(QWidget *parent = 0);

    //overrides:
    QString readCommand();
    QString readAll(int &finalLineNumber);
    QString readSentence(int &finalLineNumber);
    void printForReadCommand(const QScriptValue &message, bool last = true);
    void printOut(const QScriptValue &result, bool last = true);
    void printErr(const QScriptValue &error, bool last = true);
    void printUncaughtException(const QScriptValue &exception);
    void helpCommand();

    QString textForRead() const;
    void setTextForRead(QString readText);

signals:
    void write(QString text);

private:
    QString m_buffer;
    QString m_readBuffer;
    QWidget *m_widget;
};

#endif // SHELLCONTROLLER_H
