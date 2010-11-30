#include "consoleshell.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtCore/QTextStream>
#include <QtCore/QFile>

#include <stdio.h>
#if defined(Q_WS_WIN)
#include <io.h>
#endif

ConsoleShell::ConsoleShell(QObject *parent) :
    Shell(parent),
    m_inFile(0),
    m_in(0),
    m_err(0),
    m_out(0),
    m_mode(None),
    m_isInATty(false)
{
}

ConsoleShell::~ConsoleShell() {
    delete m_in;
    delete m_inFile;
    delete m_out;
    delete m_err;
}

QString ConsoleShell::readCommand() {
    return m_in->readLine();
}

QString ConsoleShell::readAll(int &/*finalLineNumber*/) {
    return m_in->readAll();
}

QString ConsoleShell::readLineTty(bool isContinue) {
    QString newLine = m_in->readLine();
    QString line = newLine;

    forever {
        if (newLine.length() < 1) {
            return line;
        } else if (line.at(line.length() - 1) == QChar::fromLatin1('?')) {
            line.chop(1);
            int completitionStartAt = 0;
            QString commonName;
            QStringList completitons = completeScriptExpression(line, completitionStartAt, commonName);

            int lenght = completitons.length();
            if (lenght == 1) {
                QString completition = completitons.at(0);
                QString s = line.mid(0, completitionStartAt);
                if (isContinue) {
                    *m_out << QString::fromLatin1("> ");
                } else {
                    *m_out << QString::fromLatin1("js> ");
                }
                line = s + completition;
                *m_out << s << completition;
                m_out->flush();
            } else if (lenght > 1) {
                QStringListIterator it(completitons);
                while (it.hasNext()) {
                    *m_out << it.next() << QString::fromLatin1("    ");
                }
                *m_out << QChar::fromLatin1('\n');
                if (isContinue) {
                    *m_out << QString::fromLatin1("> ");
                } else {
                    *m_out << QString::fromLatin1("js> ");
                }
                if (!commonName.isNull()) {
                    line = line + commonName;
                }
                *m_out << line;
                m_out->flush();
            } else {
                if (isContinue) {
                    *m_out << QString::fromLatin1("> ");
                } else {
                    *m_out << QString::fromLatin1("js> ");
                }
                *m_out << line;
                m_out->flush();
            }
            newLine = m_in->readLine();
            line = line + newLine;
        } else {
            return line;
        }
    }
}

QString ConsoleShell::readSentence(int &finalLineNumber) {
    const QString nl = QString::fromLatin1("\n");

    QString sentence;
    QString line;
    bool completeSentence;

    if (m_isInATty) {
        bool isContinue = false;
        printPrompt();
        do {
            line = readLineTty(isContinue);
            if (!line.isNull()) {
                sentence.append(line).append(nl);
                completeSentence = isCompleteSententence(sentence);
                if (!completeSentence) {
                    isContinue = true;
                    printContinue();
                }
            } else {
                break;
            }
        } while (!completeSentence);
        finalLineNumber = 1;
    } else {

        do {
            line = m_in->readLine();
            finalLineNumber++;
            if (!line.isNull()) {
                sentence.append(line).append(nl);
                completeSentence = isCompleteSententence(sentence);
            } else {
                break;
            }
        } while (!completeSentence);

    }

    return sentence;
}

void ConsoleShell::printForReadCommand(const QScriptValue &message, bool last) {
    printOut(message, false);
    if (last) {
        m_out->flush();
    }
}

void ConsoleShell::printOut(const QScriptValue &result, bool last) {
    const QChar sp = QChar::fromLatin1(' ');
    const QChar nl = QChar::fromLatin1('\n');

    if (last) {
        *m_out << result.toString() << nl;
    } else {
        *m_out << result.toString() << sp;
    }
}

void ConsoleShell::printErr(const QScriptValue &error, bool last) {
    const QChar sp = QChar::fromLatin1(' ');
    const QChar nl = QChar::fromLatin1('\n');

    if (last) {
        *m_err << error.toString() << nl;
    } else {
        *m_err << error.toString() << sp;
    }

    if (!m_isInATty) {
        exit(1);
    }
}

void ConsoleShell::printUncaughtException(const QScriptValue &exception) {
    if (m_isInATty) {
        m_out->flush();
    }
    QScriptEngine *eng = engine();
    QString message = QString::fromLatin1("Uncaught exception: %1. Line: %2\n")
              .arg(exception.toString())
              .arg(eng->uncaughtExceptionLineNumber());
    QStringList backtrace = eng->uncaughtExceptionBacktrace();
    foreach (QString b, backtrace) {
        message.append(QString::fromLatin1("    at %1\n").arg(b));
    }
    *m_err << message;

    if (!m_isInATty) {
        exit(1);
    }
}

void ConsoleShell::printPrompt() {
    m_err->flush();
    const QString t = QString::fromLatin1("js> ");
    *m_out << t;
    m_out->flush();
}

void ConsoleShell::printContinue() {
    const QString t = QString::fromLatin1("> ");
    *m_out << t;
    m_out->flush();
}

void ConsoleShell::helpCommand() {
    QString message = QString::fromLatin1(
            "Welcome to SRShell, version %1, a javascript shell using Qt Script\n"
            "\n"
            "Usage:\n"
            "    %2 [options]\n"
            "    %2 [options] - [arguments]\n"
            "    %2 [options] FILE [arguments]\n"
            "    %2 -c [options] SCRIPT [arguments]\n"
            "\n"
            "Description:\n"
            "    %2 run a javascript using the Qt Script engine, the script is read from\n"
            "    the standar input or a FILE (if the FILE name is - or is missing read from\n"
            "    the standar input), and pass arguments to the script. When the -c option is\n"
            "    used the script is read from SCRIPT.\n"
            "\n"
            "Options:\n"
            "    -c , -script-in-argument , --script-in-argument\n"
            "               read the script from SCRIPT argument.\n"
            "    -h , -help , --help\n"
            "               display this help with the basic usage.\n"
            "    -n , -no-global-engine , --no-global-engine\n"
            "               disable put all members of the engine in the global space.\n"
            "    -no-tty , --no-tty\n"
            "               flag to override the tty autodetectation and tell it is not\n"
            "               running inside a tty.\n"
            "    -tty , --tty\n"
            "               flag to override the tty autodetectation and tell it is running\n"
            "               inside a tty.\n"
            "\n"
            "Modes:\n"
            "    -b , -batch , --batch\n"
            "               run using the batch mode, disabiliting the interactive mode.\n"
            "               This mode load all content in the stdin to a memory and then\n"
            "               it is evaluate. This mode is used by default when a pipe is\n"
            "               detected in stdin or a file is indicated.\n"
            "    -i , -interactive , --interractive\n"
            "               run using the interractive mode, as a terminal. This this the\n"
            "               default mode, excepts when a pipe in stdin is detected or a\n"
            "               file is indicated.\n"
            "    -q , -quiet , --quiet\n"
            "               run using the batch mode, disabiliting the interactive mode.\n"
            "               This mode read the content line by line, if the line contain\n"
            "               an incomplete sentences the next line is read until get a\n"
            "               complete sentence, then it is evaluated. This mode is usefull\n"
            "               when you don't want to print in the terminal the result of the\n"
            "               sentences, or when you run a large script.\n"
            "\n"
            "%3\n"
            "\n"
            "Note:\n"
            "    * the basics commands and properties are members of the 'sr.engine' if you\n"
            "      disable put all members of the engine in the global space (using the\n"
            "      -no-global-engine option), you must use 'sr.engine.' fot get access\n"
            "       to them.\n"
            "\n"
            "Alternatives:\n"
            "    srsh      javascript shell using QtScript.\n"
            "    srshqg    javascript shell using QtScript and allowing use QtGui.\n")
    .arg(QString::fromLatin1(APP_VERSION))
    .arg(QString::fromLatin1(APP_NAME))
    .arg(helpMessage());

    *m_out << message;
}

void ConsoleShell::close(int returnCode) {
    const QChar nl = QChar::fromLatin1('\n');

    if (m_isInATty) {
        if (!isExitCalled()) {
            *m_out << nl;
        }
    }
    QCoreApplication::instance()->exit(returnCode);
}


bool ConsoleShell::init(int &retunrCode) {
    m_out = new QTextStream(stdout);
    m_err = new QTextStream(stderr);

    QStringList arguments = QCoreApplication::arguments();
    QString arg;

    bool intercative = false;
    bool batch = false;
    bool quiet = false;
    bool tty = false;
    bool noTty = false;
    bool scriptInArg;

    int argumentScriptNamePosition = -1;
    for (int i = 1; i < arguments.length(); i++) {
        arg = arguments[i];
        if (arg.startsWith(QChar::fromLatin1('-'))) {
            if (arg == QLatin1String("-help")
                    || arg == QLatin1String("--help")
                    || arg == QLatin1String("-h")) {
                helpCommand();
                retunrCode = 0;
                return true;
            } else if (arg == QLatin1String("-no-global-engine")
                    || arg == QLatin1String("--no-global-engine")
                    || arg == QLatin1String("-n")) {
                setUseGlobalEngine(false);
            } else if (arg == QLatin1String("-interactive")
                || arg == QLatin1String("--interactive")
                || arg == QLatin1String("-i")) {
                intercative = true;
            } else if (arg == QLatin1String("-batch")
                || arg == QLatin1String("--batch")
                || arg == QLatin1String("-b")) {
                batch = true;
            } else if (arg == QLatin1String("-quiet")
                || arg == QLatin1String("--quiet")
                || arg == QLatin1String("-q")) {
                quiet = true;
            } else if (arg == QLatin1String("-tty")
                || arg == QLatin1String("--tty")) {
                tty = true;
            } else if (arg == QLatin1String("-no-tty")
                || arg == QLatin1String("--no-tty")) {
                noTty = true;
            } else if (arg == QLatin1String("-script-in-argument")
                    || arg == QLatin1String("--script-in-argument")
                    || arg == QLatin1String("-c")) {
                scriptInArg = true;
            } else if (arg == QLatin1String("-")) {
                argumentScriptNamePosition = i;
                break;
            } else {
                *m_err << QString::fromLatin1("Unrecognized option '%1'.\n").arg(arg);
                helpCommand();
                retunrCode = 1;
                return true;
            }
        } else {
            argumentScriptNamePosition = i;
            break;
        }
    }

    if (intercative && batch) {
        *m_err << QString::fromLatin1("The interactive mode and the batch mode cannot be used at the saem moment.\n");
        retunrCode = 1;
        return true;
    }
    if (intercative && quiet) {
        *m_err << QString::fromLatin1("The interactive mode and the quiet mode cannot be used at the saem moment.\n");
        retunrCode = 1;
        return true;
    }
    if (batch && quiet) {
        *m_err << QString::fromLatin1("The batch mode and the quiet mode cannot be used at the saem moment.\n");
        retunrCode = 1;
        return true;
    }

    if (tty && noTty) {
        *m_err << QString::fromLatin1("The options inside in a tty y not inside in a tty canno be used at the same moment.\n");
        retunrCode = 1;
        return true;
    }

    // set the default value
    m_isInATty = isStdinTty();

    // set the default value
    if (m_isInATty) {
        m_mode = Interactive;
    } else {
        m_mode = Batch;
    }

    if (argumentScriptNamePosition > 0) {
        QString fileName = arguments[argumentScriptNamePosition];
        QStringList scriptArguments = arguments.mid(argumentScriptNamePosition);

        if (fileName == QLatin1String("-")) {
            m_in  = new QTextStream(stdin);
        } else if (scriptInArg) {
            m_script = fileName;
            m_in = new QTextStream(&m_script);
            m_mode = Batch;
            m_isInATty = false;
            setFileName(QString::fromLatin1("-"));
        } else {
            m_inFile = new QFile(fileName);
            if (m_inFile->open( QIODevice::ReadOnly)) {
                m_in  = new QTextStream(m_inFile);
                m_mode = Batch;
                m_isInATty = false;
            } else {
                *m_err << QString::fromLatin1("Unable to read the file '%1'.\n").arg(fileName);
                retunrCode = 1;
                return true;
            }
        }

        setFileName(fileName);
        setArguments(scriptArguments);
    } else {
        QString fileName = QString::fromLatin1("-");
        setFileName(fileName);
        QStringList scriptArguments;
        scriptArguments << fileName;
        setArguments(scriptArguments);
        m_in  = new QTextStream(stdin);
    }

    connect(this, SIGNAL(finished(int)), SLOT(close(int)));

    // override the default value
    if (tty) {
        m_isInATty = true;
    }
    if (noTty) {
        m_isInATty = false;
    }

    // override the default value
    if (intercative) {
        m_mode = Interactive;
    }
    if (batch) {
        m_mode = Batch;
    }
    if (quiet) {
        m_mode = Quiet;
    }

    switch(m_mode) {
    case Interactive:
        QTimer::singleShot(0, this, SLOT(runInteractive()));
        break;
    case Batch:
        QTimer::singleShot(0, this, SLOT(runBatch()));
        break;
    case Quiet:
        QTimer::singleShot(0, this, SLOT(runQuiet()));
        break;
    default:
        *m_err << QString::fromLatin1("Internal error: invalid mode.\n");
        retunrCode = 1;
        return true;
    }

    return false;
}

bool ConsoleShell::isStdinTty() {
    #if defined(Q_WS_WIN)
        return _isatty(_fileno(stdin));
    #else
        return isatty(STDIN_FILENO);
    #endif

}
