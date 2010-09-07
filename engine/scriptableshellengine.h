#ifndef SCRIPTABLESHELLENGINE_H
#define SCRIPTABLESHELLENGINE_H

#include "scriptableengine.h"
#include "shell.h"

class ScriptableShellEngine : public ScriptableEngine
{
    Q_OBJECT

public:
    explicit ScriptableShellEngine(Shell *shell, QObject *parent = 0);

    Q_INVOKABLE void help();

    Q_INVOKABLE void error();
    Q_INVOKABLE void print();
    Q_INVOKABLE QString read();

    Q_INVOKABLE void exit(int returnCode = 0);
    Q_INVOKABLE void quit();

private:
    Shell *m_shell;

};

#endif // SCRIPTABLESHELLENGINE_H
