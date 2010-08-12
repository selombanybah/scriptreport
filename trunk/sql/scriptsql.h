#ifndef SCRIPTSQL_H
#define SCRIPTSQL_H

#include <QtScript/QScriptExtensionPlugin>

class ScriptSql : public QScriptExtensionPlugin
{
public:
    ScriptSql();
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

#endif // SCRIPTSQL_H
