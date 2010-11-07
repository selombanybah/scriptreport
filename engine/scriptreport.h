#ifndef SCRIPTREPORT_H
#define SCRIPTREPORT_H

#include "scriptreportengine_global.h"

#include <QtCore/QObject>
#include <QtCore/QStringList>

class QPrinter;
class QScriptEngine;
class QTextStream;

class ScriptableReport;
class ScriptableEngine;
class TextStreamObject;
class ScriptReportEngine;

class SCRIPTREPORTENGINE_EXPORT ScriptReport : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isEditing READ isEditing WRITE setEditing)
    Q_PROPERTY(bool isFinal READ isFinal WRITE setFinal)
    Q_PROPERTY(bool isDebugging READ isDebugging WRITE setDebugging)
    Q_PROPERTY(bool isPrintErrorEnabled READ isPrintErrorEnabled WRITE setPrintErrorEnabled)
    Q_PROPERTY(QStringList arguments READ arguments WRITE setArguments)
    Q_PROPERTY(QString previousScript READ previousScript WRITE setPreviousScript)
    Q_PROPERTY(QString scriptName READ scriptName WRITE setScriptName)
    Q_PROPERTY(bool isWriteWithPrintFunctionTooEnabled READ isWriteWithPrintFunctionTooEnabled WRITE setWriteWithPrintFunctionTooEnabled)

    Q_PROPERTY(QString intermediateCode READ intermediateCode)
    Q_PROPERTY(bool hasUncaughtException READ hasUncaughtException)
    Q_PROPERTY(QString errorMessage READ errorMessage)

public:
    explicit ScriptReport(QString scriptName = QString(), QObject *parent = 0);
    explicit ScriptReport(QTextStream *inputStream, QString scriptName = QString(), QObject *parent = 0);
    explicit ScriptReport(QString input, QString scriptName = QString(), QObject *parent = 0);
    ~ScriptReport();

    bool isEditing() const;
    void setEditing(bool editing);
    bool isFinal() const;
    void setFinal(bool final);
    bool isDebugging() const;
    void setDebugging(bool debugging);
    bool isPrintErrorEnabled() const;
    void setPrintErrorEnabled(bool isPrintErrorEnabled);

    QStringList arguments() const;
    void setArguments(QStringList arguments);
    QString previousScript() const;
    void setPreviousScript(QString previousScript);

    QString scriptName() const;
    void setScriptName(QString scriptName);

    bool isWriteWithPrintFunctionTooEnabled() const;
    void setWriteWithPrintFunctionTooEnabled(bool isWriteWithPrintFunctionTooEnabled);

    TextStreamObject* input() const;
    TextStreamObject* outputHeader() const;
    TextStreamObject* output() const;
    TextStreamObject* outputFooter() const;
    TextStreamObject* printOutput() const;

    QScriptEngine* scriptEngine() /*const*/;
//   void setScriptEngine(QScriptEngine *socriptEngine);

    ScriptReportEngine* scriptReportEngine() /*const*/;

    QString intermediateCode() const;
    bool hasUncaughtException() const;
    QString errorMessage() const;

    void loadPrintConfiguration(QPrinter *printer);

public slots:
    void updateIntermediateCode();
    void run();
    void print(QPrinter *printer);

protected:
    void initEngine();

private:
    void construct();
    void initScriptReportEngine();

private:
    bool m_isPrintErrorEnabled;

    bool m_isRunRequired;
    bool m_isUpdateIntermediateCodeRequired;
    bool m_isInitialized;

    bool m_isInEditingMode;
    bool m_isInDebuggingMode;

    QString m_previousScript;
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
