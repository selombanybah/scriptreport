#ifndef SCRIPTREPORTTOOL_H
#define SCRIPTREPORTTOOL_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

class QFile;
class QTextStream;
class QPrinter;

class ScriptReport;

class ScriptReportTool : public QObject
{
    Q_OBJECT

public:
    explicit ScriptReportTool(QObject *parent = 0);
    ~ScriptReportTool();

    bool init(int &retunrCode);
    void printHelp();

public slots:
    void run();
    void runPreview();
    void compile();

private:
    QString m_fileName;
    QString m_previousScript;
    QStringList m_scriptArguments;
    QString m_compiledFileNane;
    bool m_preview;

    QFile *m_inFile;
    QFile *m_outFile;
    QTextStream *m_in;
    QTextStream *m_err;
    QTextStream *m_out;

    bool m_editing;
    bool m_debugging;
    bool m_printError;
};

#endif // SCRIPTREPORTTOOL_H
