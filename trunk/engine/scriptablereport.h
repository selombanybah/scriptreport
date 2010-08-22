#ifndef SCRIPTABLEREPORT_H
#define SCRIPTABLEREPORT_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtScript/QScriptable>

#include "scriptablepaper.h"

class QScriptEngine;
class QPrinter;

class ScriptReportEngine;
class TextStreamObject;

class ScriptableReport : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString page READ page)
    Q_PROPERTY(QString pageCount READ pageCount)
    Q_PROPERTY(QString reportEngineVersion READ reportEngineVersion)
    Q_PROPERTY(bool isFinal READ isFinal)
    Q_PROPERTY(bool isEditing READ isEditing)
    Q_PROPERTY(bool isDebbuging READ isDebbuging)
    Q_PROPERTY(QStringList availableExtensions READ availableExtensions)
    Q_PROPERTY(QStringList importedExtensions READ importedExtensions)
    Q_PROPERTY(ScriptablePaper* paper READ paper)

public:
    ScriptableReport(ScriptReportEngine *simpleTemplateEngine, QObject *parent = 0);

    QString title();
    void setTitle(QString title);

    QString page();
    QString pageCount();
    QString reportEngineVersion();
    bool isFinal();
    bool isEditing();
    bool isDebbuging();
    QStringList availableExtensions();
    QStringList importedExtensions();
    ScriptablePaper* paper();

    Q_INVOKABLE void writeHeader();
    Q_INVOKABLE void writeContent();
    Q_INVOKABLE void writeFooter();
    Q_INVOKABLE void importExtension(QString name);

    void loadConfigurationFrom(QPrinter &printer);
    void applyConfigurationTo(QPrinter &printer);
    void initEngine(QScriptEngine &engine);

private:
    void writeResult(TextStreamObject *outputObject);
    void printAndWriteResult(TextStreamObject *outputObject);

private:
    ScriptReportEngine *m_sre;
    ScriptablePaper *m_scriptablePaper;
    QString m_title;
};

#endif // SCRIPTABLEREPORT_H
