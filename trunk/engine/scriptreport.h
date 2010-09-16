#ifndef SCRIPTREPORT_H
#define SCRIPTREPORT_H

#include "scriptreportengine_global.h"

#include <QtCore/QObject>

class QPrinter;
class QScriptEngine;
class QTextStream;

class ScriptableReport;
class ScriptableEngine;
class TextStreamObject;
class ScriptReportEngine;

class SCRIPTREPORTENGINE_EXPORT ScriptReport
{

public:
    explicit ScriptReport(QString scriptName = QString());
    explicit ScriptReport(QTextStream *inputStream, QString scriptName = QString());
    explicit ScriptReport(QString input, QString scriptName = QString());
    ~ScriptReport();
    bool run();

    bool isEditing() const;
    bool isFinal() const;
    void setEditing(bool editing);
    bool isDebugging() const;
    void setDebugging(bool debugging);
    bool isPrintErrorEnabled() const;
    void setPrintErrorEnabled(bool isPrintErrorEnabled);

    QStringList arguments() const;
    void setArguments(QStringList arguments);

    QString scriptName() const;
    void setScriptName(QString scriptName);

    bool isWriteWithPrintFunctionTooEnabled() const;
    void setWriteWithPrintFunctionTooEnabled(bool isWriteWithPrintFunctionTooEnabled);

    TextStreamObject* input() const;
    TextStreamObject* outputHeader() const;
    TextStreamObject* output() const;
    TextStreamObject* outputFooter() const;
    TextStreamObject* print() const;

    QScriptEngine* scriptEngine() /*const*/;
//   void setScriptEngine(QScriptEngine *scriptEngine);

    ScriptReportEngine* scriptReportEngine() /*const*/;

    QString intermediateCode() const;
    void updateIntermediateCode();

    QString errorMessage() const;

    void loadPrintConfiguration(QPrinter *printer);
    void print(QPrinter *printer);

protected:
    void initEngine();

private:
    void construct();

private:
    bool m_isPrintErrorEnabled;

    bool m_isRunRequired;
    bool m_isUpdateIntermediateCodeRequired;
    bool m_isInitialized;

    bool m_isInEditingMode;
    bool m_isInDebuggingMode;

    QString m_name;
    QString m_type;
    bool m_isWriteWithPrintFunctionTooEnabled;
    QScriptEngine *m_engine;
    QString m_intermediate;

    TextStreamObject *m_inStreamObject;
    TextStreamObject *m_outHeaderStreamObject;
    TextStreamObject *m_outStreamObject;
    TextStreamObject *m_outFooterStreamObject;
    TextStreamObject *m_printStreamObject;

    ScriptableReport *m_scriptableReport;
    ScriptableEngine *m_scriptableEngine;

    ScriptReportEngine *m_scriptReportEngine;
};

#endif // SCRIPTREPORT_H
