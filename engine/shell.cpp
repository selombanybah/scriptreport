#include "shell.h"

#include <QtScript/QScriptValueIterator>

#include "scriptable/scriptableshellengine.h"

/*
 * Static
 */

static QScriptValue print(QScriptContext *context, QScriptEngine *engine) {
    QScriptValue calleeData = context->callee().data();
    Shell *shell = qobject_cast<Shell*>(calleeData.toQObject());
    int argumentCount = context->argumentCount();

    if (argumentCount > 0) {
        for (int i = 0; i < argumentCount - 1; i++) {
            QScriptValue value = context->argument(i);
            shell->printOut(value, false);
        }
        QScriptValue value = context->argument(argumentCount - 1);
        shell->printOut(value);
    }

    return engine->undefinedValue();
}

/*
 * Class
 */

Shell::Shell(QObject *parent) :
    QObject(parent),
    m_lineNumber(1),
    m_isInitialized(false),
    m_useGlobalEngine(true),
    m_exit(false),
    m_exitCode(0),
    m_fileName(QString()),
    m_scriptableEngine(0)
{
    m_engine = new QScriptEngine(this);
}

Shell::~Shell() {
}

void Shell::printResult(const QScriptValue &result) {
    printOut(result);
}

void Shell::printUncaughtException(const QScriptValue &exception) {
    printErr(exception);
}

void Shell::initEngine(QScriptEngine &engine) {
    if (!m_scriptableEngine) {
        m_scriptableEngine = new ScriptableShellEngine(this, this);
    }

    QScriptValue global = engine.globalObject();

    if (m_useGlobalEngine) {
        QScriptValue globalEngine = engine.newQObject(m_scriptableEngine, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);

        QScriptValueIterator it(global);
        while (it.hasNext()) {
            it.next();
            // we provide our own print implementation, but we want the rest
            if (it.name() != QLatin1String("print")) {
                globalEngine.setProperty(it.name(), it.value(), it.flags());
            }
        }
        global = globalEngine;
        engine.setGlobalObject(globalEngine);
    } else {
        // replace the default print
        QScriptValue printFunction = engine.newFunction(print);
        printFunction.setData(engine.newQObject(this));
        engine.globalObject().setProperty(QString::fromLatin1("print"), printFunction, QScriptValue::Undeletable);
    }

    QScriptValue sr = engine.newObject();
    global.setProperty(QString::fromLatin1("sr"), sr, QScriptValue::Undeletable);

    QScriptValue eng = engine.newQObject(m_scriptableEngine, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
    sr.setProperty(QString::fromLatin1("engine"), eng, QScriptValue::Undeletable);
}

void Shell::runInteractive() {
    if (!m_isInitialized) {
        m_isInitialized = true; // after for prevent an indirect recursive call
        initEngine(*m_engine);
    }
    m_exit = false;
    m_exitCode = 0;

    QString sentence;
    int previousLine = m_lineNumber;
    do {
        sentence = readSentence(m_lineNumber);

        QScriptValue result = m_engine->evaluate(sentence, m_fileName, previousLine);
        previousLine = m_lineNumber;
        if (m_engine->hasUncaughtException()) {
            printUncaughtException(result);
        } else if (!result.isUndefined()) {
            printResult(result);
        }

    } while (!sentence.isNull() && !m_exit);

    if (!m_exit) {
        emit finished(0);
    }
}

void Shell::runQuiet() {
    if (!m_isInitialized) {
        m_isInitialized = true; // after for prevent an indirect recursive call
        initEngine(*m_engine);
    }
    m_exit = false;
    m_exitCode = 0;

    QString sentence;
    int previousLine = m_lineNumber;
    do {
        sentence = readSentence(m_lineNumber);

        QScriptValue result = m_engine->evaluate(sentence, m_fileName, previousLine);
        previousLine = m_lineNumber;
        if (m_engine->hasUncaughtException()) {
            printUncaughtException(result);
        }

    } while (!sentence.isNull() && !m_exit);

    if (!m_exit) {
        emit finished(0);
    }
}

void Shell::runBatch() {
    if (!m_isInitialized) {
        m_isInitialized = true; // after for prevent an indirect recursive call
        initEngine(*m_engine);
    }
    m_exit = false;
    m_exitCode = 0;

    int previousLine = m_lineNumber;
    QString script = readAll(m_lineNumber);

    QScriptValue result = m_engine->evaluate(script, m_fileName, previousLine);
    if (m_engine->hasUncaughtException()) {
        printUncaughtException(result);
    }

    if (!m_exit) {
        emit finished(0);
    }
}

void Shell::runOneSentence() {
    if (!m_isInitialized) {
        m_isInitialized = true; // after for prevent an indirect recursive call
        initEngine(*m_engine);
    }
    m_exit = false;
    m_exitCode = 0;

    int previousLine = m_lineNumber;
    QString sentence = readSentence(m_lineNumber);

    QScriptValue result = m_engine->evaluate(sentence, m_fileName, previousLine);
    if (m_engine->hasUncaughtException()) {
        printUncaughtException(result);
    }
}

void Shell::runOneSentenceInteractive() {
    if (!m_isInitialized) {
        m_isInitialized = true; // after for prevent an indirect recursive call
        initEngine(*m_engine);
    }
    m_exit = false;
    m_exitCode = 0;

    int previousLine = m_lineNumber;
    QString sentence = readSentence(m_lineNumber);

    QScriptValue result = m_engine->evaluate(sentence, m_fileName, previousLine);
    if (m_engine->hasUncaughtException()) {
        printUncaughtException(result);
    } else if (!result.isUndefined()) {
        printResult(result);
    }
}

bool Shell::isCompleteSententence(QString sentence) {
    QScriptSyntaxCheckResult check = QScriptEngine::checkSyntax(sentence);
    return check.state() != QScriptSyntaxCheckResult::Intermediate;
}

void Shell::exit(int exitCode) {
    m_exit = true;
    m_exitCode = exitCode;
    emit finished(exitCode);
}

int Shell::exitCode() const {
    return m_exitCode;
}

QString Shell::helpMessage() {
    QString message = QString::fromLatin1(
            "List of basics commands:\n"
            "    error([message, ...])    print a message in the standard error.\n"
            "    exit()                   exit the shell with return code 0 (zero).\n"
            "    exit(code)               exit the shell with return number code pased as\n"
            "                             the argument 'code'.\n"
            "    getFromEnvironment(name) search for a variable identified by 'name'' and\n"
            "                             returns its value or null if is not found.\n"
            "    help()                   display this help with the basic usage.\n"
            "    importExtension([name, ...]) load the extension(s) with name 'name'.\n"
            "    load([filename, ...])    load and excute the javascrip file(s) with name\n"
            "                             'name' inside the current shell.\n"
            "    print([message, ...])    print a mensagge(s) in the standard output.\n"
            "    quit()                   exit the shell with return code 0 (zero).\n"
            "    read([message, ...])     read a line from the standard input. If a message\n"
            "                             this will be printed an then will read the input.\n"
            "    readFile(name)           read all content of a file.\n"
            "    runCommand(commandName, [arg, ...] [options]) execute the specified command\n"
            "                             with the given argument and options as a separate\n"
            "                             process and return the exit status of the process.\n"
            "                             See runCommand usage for more detail.\n"
            "    runCommand(options)      execute the specified command in the options object\n"
            "                             as a separate process and return the exit status of\n"
            "                             the process. See runCommand usage for more detail.\n"
            "\n"
            "List of basic properties (exposed as a readonly variables):\n"
            "    arguments                list of all argumets given at the command line\n"
            "                             when the shell was invoked.\n"
            "    availableExtensions      list of all avaiable QtScript' extensions.\n"
            "    importedExtensions       list of all imported QtScript' extensions.\n"
            "    qtVersion                version of the Qt library in use.\n"
            "    srVersion                version of the shell, value %1.\n"
            "\n"
            "runCommand() usage:\n"
            "    runCommand(command)\n"
            "    runCommand(command, arg1, ..., argN)\n"
            "    runCommand(command, arg1, ..., argN, options)\n"
            "    runCommand(options)\n"
            "\n"
            "    All except the last arguments to runCommand are converted to strings and\n"
            "    denote command name and its arguments. If the last argument is a JavaScript\n"
            "    object, it is an option object. Otherwise it is converted to string denoting\n"
            "    the last argument and options objects assumed to be empty.\n"
            "\n"
            "    The following properties of the option object are processed:\n"
            "    * args     provides an array of additional command arguments\n"
            "    * env      A JavaScript object with the properties to add to the process\n"
            "               environment. All its enumeratable properties define the\n"
            "               corresponding environment variable names.\n"
            "    * input    the process input. A string sent to the process as its input.\n"
            "               If not specified, no input is provided to the process.\n"
            "    * output   the process output. If it is specified, the process output is\n"
            "               read, converted to a string, appended to the output property\n"
            "               value converted to string and put as the new value of the output\n"
            "               property.\n"
            "    * err      the process error output. If it is specified, the process error\n"
            "               output is read, converted to a string, appended to the err\n"
            "               property value converted to string and put as the new value of\n"
            "               the err property.\n"
            "    * command  command name. Only used if the command name is not passed as\n"
            "               argument of the runCommand method.\n"
            "    * result   exit statud of the process. If it is specified, the exit statud\n"
            "               of the process will be stetted in this property.")
        .arg(QString::fromLatin1(APP_VERSION));
    return message;
}

bool Shell::useGlobalEngine() const {
    return m_useGlobalEngine;
}

void Shell::setUseGlobalEngine(bool useGlobalEngine) {
    m_useGlobalEngine = useGlobalEngine;
}

QStringList Shell::arguments() const {
    if (!m_scriptableEngine) {
        return m_scriptableEngine->arguments();
    } else {
        return QStringList();
    }
}

void Shell::setArguments(QStringList arguments) {
    if (!m_scriptableEngine) {
        m_scriptableEngine = new ScriptableShellEngine(this, this);
    }
    m_scriptableEngine->setArguments(arguments);
}

QString Shell::fileName() const {
    return m_fileName;
}

void Shell::setFileName(QString fileName) {
    m_fileName = fileName;
}

int Shell::currentLineNumber() const {
    return m_lineNumber;
}

void Shell::setCurrentLineNumber(int currentLineNumber) {
    m_lineNumber = currentLineNumber;
}

int Shell::processEventsInterval() const {
    return m_engine->processEventsInterval();
}

void Shell::setProcessEventsInterval(int interval) {
    m_engine->setProcessEventsInterval(interval);
}

bool Shell::isExitCalled() const {
    return m_exit;
}

bool Shell::isEngineInitialized() const {
    return m_isInitialized;
}

QScriptEngine* Shell::engine() {
    if (!m_isInitialized) {
        m_isInitialized = true; // after for prevent an indirect recursive call
        initEngine(*m_engine);
    }
    return m_engine;
}

void Shell::reset() {
    m_lineNumber = 1;
    int interval = m_engine->processEventsInterval();
    delete m_engine;
    m_engine = new QScriptEngine(this);
    m_engine->setProcessEventsInterval(interval);
    m_isInitialized = false;
    m_exit = false;
}

static bool isIdentifierChar(const QChar &ch)
{
    static QChar underscore = QChar::fromLatin1('_');
    return ch.isLetter() || ch.isNumber() || (ch == underscore);
}

static QStringList findCompletions(QScriptContext *context, QStringList path, QString name, QString &commonName) {
    QScriptValue v = context->thisObject();
    int lenght = path.length();

    if (lenght > 0) {
        int pos = 0;
        if (path.at(pos) == QString::fromLatin1("this")) {
            pos++;
        }

        for( ; pos < lenght; pos++) {
            v = v.property(path.at(pos));
        }
    }

    QStringList result;
    if (name.isEmpty()) {
        QScriptValueIterator it(v);
        while (it.hasNext()) {
            it.next();
            result << it.name();
        }
        if (lenght == 0) {
            result << QString::fromLatin1("this");
            result << QString::fromLatin1("true");
            result << QString::fromLatin1("false");
            result << QString::fromLatin1("null");
        }
    } else {

        QString common;
        QScriptValueIterator it(v);
        while (it.hasNext()) {
            it.next();
            QString propertyName = it.name();
            if (propertyName.startsWith(name)) {
                result << propertyName;
                if (common.isNull()) {
                    common = propertyName;
                } else {
                    int i = 0;
                    for( ; i < common.length() && i < propertyName.length(); i++) {
                        if (common.at(i) != propertyName.at(i)) {
                            break;
                        }
                    }
                    common = common.mid(0, i);
                }
            }
        }
        if (!common.isNull()) {
            commonName = common.mid(name.length());
        }
        if (lenght == 0) {
            QString word = QString::fromLatin1("this");
            if (word.startsWith(name)) {
                result << word;
            }
            word = QString::fromLatin1("true");
            if (word.startsWith(name)) {
                result << word;
            }
            word = QString::fromLatin1("false");
            if (word.startsWith(name)) {
                result << word;
            }
            word = QString::fromLatin1("null");
            if (word.startsWith(name)) {
                result << word;
            }
        }
    }

    qStableSort(result);
    return result;
}

QStringList Shell::completeScriptExpression(QString expression, int &completitionStartAt, QString &commonName) {
    if (!m_isInitialized) {
        m_isInitialized = true; // after for prevent an indirect recursive call
        initEngine(*m_engine);
    }

    QStringList path;
    QString name;

    if (expression.isEmpty()) {
        completitionStartAt = 0;
        return findCompletions(m_engine->currentContext(), path, name, commonName);
    }

    int endWord = expression.length() - 1;
    int startWord = endWord + 1;
    QChar dot = QChar::fromLatin1('.');

    while (startWord > 0 && isIdentifierChar(expression.at(startWord - 1)) ) {
        startWord--;
    }
    if (startWord <= endWord) {
        name = expression.mid(startWord, endWord - startWord + 1);
    }
    completitionStartAt = startWord;
    startWord--;

    while (startWord > 0 && expression.at(startWord) == dot) {
        endWord = startWord;
        while (startWord > 0 && isIdentifierChar(expression.at(startWord - 1)) ) {
            startWord--;
        }
        QString pathItem = expression.mid(startWord, endWord - startWord);
        path.prepend(pathItem);
        startWord--;
    }

    return findCompletions(m_engine->currentContext(), path, name, commonName);
}

QString Shell::version() const {
    return QString::fromLatin1(APP_VERSION);
}
