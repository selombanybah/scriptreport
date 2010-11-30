#include "scriptreporttool.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtCore/QTextStream>

#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QPrintPreviewDialog>

#include <QtScript/QScriptEngine>

#include <ScriptReport/ScriptReport>
#include <ScriptReport/TextStreamObject>

ScriptReportTool::ScriptReportTool(QObject *parent) :
    QObject(parent),
    m_preview(false),
    m_inFile(0),
    m_outFile(0),
    m_in(0),
    m_err(0),
    m_out(0),
    m_editing(false),
    m_debugging(false),
    m_printError(false)
{
}

ScriptReportTool::~ScriptReportTool() {
    delete m_in;
    delete m_inFile;
    delete m_out;
    delete m_outFile;
    delete m_err;
}

void ScriptReportTool::printHelp() {
    QString message = QString::fromLatin1(
            "Welcome to Script Report Tool, version %1\n"
            "\n"
            "Usage:\n"
            "    %2 [options]\n"
            "    %2 [options] - [arguments]\n"
            "    %2 [options] FILE [arguments]\n"
            "    %2 -c FILE OUTPUT\n"
            "    %2 -c FILE -\n"
            "    %2 -c - OUTPUT\n"
            "    %2 -c - -\n"
            "\n"
            "Description:\n"
            "    %2 run a script report, the script is read from the standar\n"
            "    input or a FILE (if the FILE name is - or is missing read from the standar\n"
            "    input), and pass arguments to the script. When the -c option is used the\n"
            "    script report is compiled to javascript and writed to OUTPUT file name, if\n"
            "    OUTPUT is missing the output file name is generated, if OUTPUT is - the\n"
            "    generated javascript write in the standard output.\n"
            "\n"
            "Options:\n"
            "    -c , -compile , --compile\n"
            "               compile the script report to javascript.\n"
            "    -e , -editing , --editing\n"
            "               enable the editing mode.\n"
            "    -d , -debugging , --debugging\n"
            "               enable the debugging mode.\n"
            "    -h , -help , --help\n"
            "               display this help with the basic usage.\n"
            "    -p , -preview , --preview\n"
            "               show the print preview.\n"
            "    -r , -print-error , --print-error\n"
            "               print the error if it was happened. If an error happened with\n"
            "               this option print the error in an extra page.\n"
            "    -s SCRIPT, -previous-script SCRIPT, --previous-script SCRIPT\n"
            "               run the SCRIPT previously of run the script report.\n")
        .arg(QString::fromLatin1(APP_VERSION))
        .arg(QString::fromLatin1(APP_NAME));

    *m_out << message;
}

bool ScriptReportTool::init(int &retunrCode) {
    m_out = new QTextStream(stdout);
    m_err = new QTextStream(stderr);

    QStringList arguments = QCoreApplication::arguments();
    QString arg;

    bool compile = false;
    bool previousScript = false;

    int argumentScriptNamePosition = -1;
    for (int i = 1; i < arguments.length(); i++) {
        arg = arguments[i];
        if (arg.startsWith(QChar::fromLatin1('-'))) {
            if (arg == QLatin1String("-help")
                    || arg == QLatin1String("--help")
                    || arg == QLatin1String("-h")) {
                printHelp();
                retunrCode = 0;
                return true;
            } else if (arg == QLatin1String("-editing")
                    || arg == QLatin1String("--editing")
                    || arg == QLatin1String("-e")) {
                m_editing = true;
            } else if (arg == QLatin1String("-debugging")
                    || arg == QLatin1String("--debugging")
                    || arg == QLatin1String("-d")) {
                m_debugging = true;
            } else if (arg == QLatin1String("-compile")
                    || arg == QLatin1String("--compile")
                    || arg == QLatin1String("-c")) {
                compile = true;
            } else if (arg == QLatin1String("-preview")
                    || arg == QLatin1String("--preview")
                    || arg == QLatin1String("-p")) {
                m_preview = true;
            } else if (arg == QLatin1String("-print-error")
                    || arg == QLatin1String("--print-error")
                    || arg == QLatin1String("-r")) {
                m_printError = true;
            } else if (arg == QLatin1String("-previous-script")
                    || arg == QLatin1String("--previous-script")
                    || arg == QLatin1String("-s")) {
                i++;
                previousScript = true;
                m_previousScript = arguments[i];
            } else if (arg == QLatin1String("-")) {
                argumentScriptNamePosition = i;
                break;
            } else {
                *m_err << QString::fromLatin1("Unrecognized option '%1'.\n").arg(arg);
                printHelp();
                retunrCode = 1;
                return true;
            }
        } else {
            argumentScriptNamePosition = i;
            break;
        }
    }

    if (compile && previousScript) {
        *m_err << QString::fromLatin1("The compile mode cannot has previous script.\n");
        retunrCode = 1;
        return true;
    }

    if (compile && m_preview) {
        *m_err << QString::fromLatin1("The compile mode cannot has preview.\n");
        retunrCode = 1;
        return true;
    }

    if (compile && m_printError) {
        *m_err << QString::fromLatin1("The compile mode cannot has print error setting.\n");
        retunrCode = 1;
        return true;
    }

    if (compile && m_editing) {
        *m_err << QString::fromLatin1("The compile mode cannot has edditing setting.\n");
        retunrCode = 1;
        return true;
    }

    if (compile && m_debugging) {
        *m_err << QString::fromLatin1("The compile mode cannot has debugging setting.\n");
        retunrCode = 1;
        return true;
    }

    if (argumentScriptNamePosition > 0) {
        m_fileName = arguments[argumentScriptNamePosition];
        m_scriptArguments = arguments.mid(argumentScriptNamePosition + 1);

        if (m_fileName == QLatin1String("-")) {
            m_in  = new QTextStream(stdin);
        } else {

            m_inFile = new QFile(m_fileName);
            if (m_inFile->open( QIODevice::ReadOnly)) {
                m_in  = new QTextStream(m_inFile);

                QFileInfo fileInfo(m_fileName);
                QDir::setCurrent(fileInfo.absolutePath());
            } else {
                *m_err << QString::fromLatin1("Unable to read the file '%1'.\n").arg(m_fileName);
                retunrCode = 2;
                return true;
            }
        }

        if (compile) {
            argumentScriptNamePosition++;
            if (argumentScriptNamePosition < arguments.length()) {
                m_compiledFileNane = arguments[argumentScriptNamePosition];
            }
            if (m_compiledFileNane.isEmpty()) {
                if (m_inFile) {
                    QFileInfo fi(m_fileName);
                    m_compiledFileNane = fi.fileName().append(QLatin1String(".js"));
                } else {
                    m_compiledFileNane = QString::fromLatin1("out.js");
                }
            }

            if (m_compiledFileNane != QLatin1String("-")) {
                delete m_out;
                m_outFile = new QFile(m_compiledFileNane);
                if (m_outFile->open(QIODevice::WriteOnly)) {
                    m_out  = new QTextStream(m_outFile);
                } else {
                    *m_err << QString::fromLatin1("Unable to open the output file '%1'.\n").arg(m_compiledFileNane);
                    retunrCode = 3;
                    return true;
                }
            }
        }
    } else {
        *m_err << QString::fromLatin1("Missing file name.\n");
        retunrCode = 1;
        return true;
    }

    if (compile) {
        QTimer::singleShot(0, this, SLOT(compile()));
    } else if (m_preview) {
         QTimer::singleShot(0, this, SLOT(runPreview()));
    } else {
        QTimer::singleShot(0, this, SLOT(run()));
    }

    return false;
}

void ScriptReportTool::run() {
    QPrinter printer;
    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Accepted) {
        ScriptReport sr(m_in, m_fileName);
        sr.setArguments(m_scriptArguments);
        sr.setPreviousScript(m_previousScript);
        sr.setEditing(m_editing);
        sr.setDebugging(m_debugging);
        sr.printOutput()->setStream(m_out);
        sr.run();
        bool hasUncaughtException = sr.hasUncaughtException();
        if (!hasUncaughtException || m_printError) {
            m_out->flush();
            if (hasUncaughtException) {
                *m_err << QString::fromLatin1("Error: %1\n").arg(sr.errorMessage());
                m_err->flush();
            }
            sr.print(&printer);
            if (hasUncaughtException) {
                QCoreApplication::exit(6);
            }
        } else {
            *m_err << QString::fromLatin1("Error: %1\n").arg(sr.errorMessage());
            m_out->flush();
            m_err->flush();
            QCoreApplication::exit(6);
        }
    }
    QCoreApplication::exit(0);
}

void ScriptReportTool::runPreview() {
    ScriptReport *sr = new ScriptReport(m_in, m_fileName);
    sr->setArguments(m_scriptArguments);
    sr->setPreviousScript(m_previousScript);
    sr->setEditing(m_editing);
    sr->setDebugging(m_debugging);
    sr->printOutput()->setStream(m_out);
    sr->run();
    bool hasUncaughtException = sr->hasUncaughtException();
    if (!hasUncaughtException || m_printError) {
        m_out->flush();
        if (hasUncaughtException) {
            *m_err << QString::fromLatin1("Error: %1\n").arg(sr->errorMessage());
            m_err->flush();
        }
        QPrintPreviewDialog *p = new QPrintPreviewDialog();
        connect(p, SIGNAL(paintRequested(QPrinter*)), sr, SLOT(print(QPrinter*)));
        p->exec();
        delete p;
        p = 0;
        delete sr;
        if (hasUncaughtException) {
            QCoreApplication::exit(6);
        }
    } else {
        *m_err << QString::fromLatin1("Error: %1\n").arg(sr->errorMessage());
        m_out->flush();
        m_err->flush();
        delete sr;
        QCoreApplication::exit(6);
    }
    QCoreApplication::exit(0);
}

void ScriptReportTool::compile() {
    ScriptReport sr(m_in, m_fileName);
    sr.updateIntermediateCode();
    *m_out << sr.intermediateCode();
    m_out->flush();

    // Validate the script
    QScriptSyntaxCheckResult result = QScriptEngine::checkSyntax(sr.intermediateCode());
    QScriptSyntaxCheckResult::State state = result.state();
    switch (state) {
    case QScriptSyntaxCheckResult::Error:
        *m_err << QString::fromLatin1("Error: Line %1, Column %2: %3.")
                  .arg(QString::number(result.errorLineNumber()),
                       QString::number(result.errorColumnNumber()),
                       result.errorMessage());
        m_err->flush();
        QCoreApplication::exit(4);
    case QScriptSyntaxCheckResult::Intermediate:
        *m_err << QString::fromLatin1("Error: The report is incomplete.");
        m_err->flush();
        QCoreApplication::exit(5);
    case QScriptSyntaxCheckResult::Valid:
        QCoreApplication::exit(0);
        break;
    default:
        QCoreApplication::exit(0);
    }

    return;
}
