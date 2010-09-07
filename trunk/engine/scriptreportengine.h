#ifndef SCRIPTREPORTENGINE_H
#define SCRIPTREPORTENGINE_H

#include "scriptreportengine_global.h"

#include <QtCore/QObject>

class QPrinter;
class QScriptEngine;
class QTextStream;

class ScriptableReport;
class ScriptableEngine;
class TextStreamObject;

class SCRIPTREPORTENGINE_EXPORT ScriptReportEngine
{
public:
    explicit ScriptReportEngine(QString scriptName = QString(), bool isPrintErrorEnabled = true, bool writeWithPrintFunction = false);
    explicit ScriptReportEngine(QTextStream *inputStream, QString scriptName = QString(), bool isPrintErrorEnabled = true, bool writeWithPrintFunction = false);
    explicit ScriptReportEngine(QString input, QString scriptName = QString(), bool isPrintErrorEnabled = true, bool writeWithPrintFunction = false);
    ~ScriptReportEngine();
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

    QScriptEngine *scriptEngine() /*const*/;
 //   void setScriptEngine(QScriptEngine *scriptEngine);

    QString intermediateCode() const;
    void updateIntermediateCode();

    QString errorMessage() const;

    virtual void loadPrintConfiguration(QPrinter &printer);
    virtual void print(QPrinter *printer);

protected:
    virtual void initEngine(QScriptEngine &se);

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
};

#endif // SCRIPTREPORTENGINE_H
