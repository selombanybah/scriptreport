#ifndef SCRIPTREPORTI18N_H
#define SCRIPTREPORTI18N_H

#include <QtScript/QScriptExtensionPlugin>

class ScriptReportI18N : public QScriptExtensionPlugin
{
public:
    ScriptReportI18N();
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

#endif // SCRIPTREPORTI18N_H
