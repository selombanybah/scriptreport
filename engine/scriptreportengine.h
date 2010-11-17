#ifndef SCRIPTREPORTENGINE_H
#define SCRIPTREPORTENGINE_H

#include "scriptreportengine_global.h"

#include <QtCore/QObject>
#include "scriptreport.h"

class QPrinter;
class QScriptEngine;

class ScriptReportEnginePrivate;

class SCRIPTREPORTENGINE_EXPORT ScriptReportEngine
{
public:
    ScriptReportEngine();
    virtual ~ScriptReportEngine();

    virtual void loadPrintConfiguration(ScriptReport *scriptReport, QPrinter *printer);
    virtual void print(ScriptReport *scriptReport, QPrinter *printer);
    virtual void initEngine(ScriptReport *scriptReport, QScriptEngine *engine);

private:
    ScriptReportEnginePrivate *d;
};

#endif // SCRIPTREPORTENGINE_H
