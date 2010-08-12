#ifndef SCRIPTREPORTENGINE_H
#define SCRIPTREPORTENGINE_H

#include "scriptreportengine_global.h"

#include <QtCore/QObject>

class QPrinter;
class QScriptEngine;
class QTextStream;

class ScriptableReport;
class TextStreamObject;

class SCRIPTREPORTENGINE_EXPORT ScriptReportEngine
{
public:
    explicit ScriptReportEngine(QString scriptName = QString(), bool printResult = false);
    explicit ScriptReportEngine(QTextStream *inputStream, QString scriptName = QString(), bool printResult = false);
    explicit ScriptReportEngine(QString input, QString scriptName = QString(), bool printResult = false);
    ~ScriptReportEngine();
    bool run();

    bool isEditing();
    bool isFinal();
    void setEditing(bool editing);
    bool isDebugging();
    void setDebugging(bool debugging);

    QString scriptName();
    void setScriptName(QString scriptName);

    bool isPrintResultEnabled();
    void setPrintResultEnabled(bool printResult);

    TextStreamObject* input() const;
    TextStreamObject* outputHeader() const;
    TextStreamObject* output() const;
    TextStreamObject* outputFooter() const;
    TextStreamObject* print() const;

    QScriptEngine *scriptEngine() /*const*/;
 //   void setScriptEngine(QScriptEngine *scriptEngine);

    QString intermediateCode() const;
    void updateIntermediateCode();

    void loadPrintConfiguration(QPrinter &printer);
    void print(QPrinter *printer);

protected:
    virtual void initEngine(QScriptEngine &se);

private:
    void construct();

private:
    bool m_isUpdateIntermediateCodeRequired;
    bool m_isInitialized;

    bool m_isInEditingMode;
    bool m_isInDebuggingMode;

    QString m_name;
    bool m_isPrintAndWriteEnabled;
    QScriptEngine *m_engine;
    QString m_intermediate;

    TextStreamObject *m_inStreamObject;
    TextStreamObject *m_outHeaderStreamObject;
    TextStreamObject *m_outStreamObject;
    TextStreamObject *m_outFooterStreamObject;
    TextStreamObject *m_printStreamObject;

    ScriptableReport *m_scriptableReport;
};

#endif // SCRIPTREPORTENGINE_H
