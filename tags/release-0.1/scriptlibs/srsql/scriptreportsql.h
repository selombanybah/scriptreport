#ifndef SCRIPTSQL_H
#define SCRIPTSQL_H

#include <QtScript/QScriptExtensionPlugin>

class ScriptReportSql : public QScriptExtensionPlugin
{
public:
    ScriptReportSql();
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

#endif // SCRIPTSQL_H
