#include "scriptablei18n.h"

#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptContextInfo>
#include <QtCore/QFileInfo>
#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QMetaEnum>

ScriptableI18N::ScriptableI18N(QObject *parent) :
    QObject(parent),
    cachedTranslationContext(0)
{
}

QString ScriptableI18N::currentContext() {
    return getCurrentContext(context());
}

QString ScriptableI18N::getCurrentContext(QScriptContext *context) {
    if (context == cachedTranslationContext) {
        return cachedTranslationContextName;
    }

    QScriptContext *cx = context;
    QString name;
    while (cx) {
        QScriptContextInfo info(cx);
        name = info.fileName();
        if (!name.isEmpty()) {
            name = QFileInfo(name).baseName();
            break;
        }
        cx = cx->parentContext();
    }
    cachedTranslationContext = context;
    cachedTranslationContextName = name;
    return name;
}

ScriptableTranslator *ScriptableI18N::openTranslator(const QString filename, const QString directory, const QString searchDelimiters, const QString suffix) {
    QTranslator *translator = new QTranslator();
    bool loaded = translator->load(filename, directory, searchDelimiters, suffix);
    if (loaded) {
        ScriptableTranslator *result = new ScriptableTranslator(translator);
        translator->setParent(result);
        return result;
    } else {
        delete translator;
        return 0;
    }
}

bool ScriptableI18N::installTranslator(ScriptableTranslator *translator) {
    if (!translator) {
        return false;
    }
    translator->setParent(this);
    translators.prepend(translator);
    return true;
}

ScriptableTranslator *ScriptableI18N::installTranslator(const QString filename, const QString directory, const QString searchDelimiters, const QString suffix) {
    QTranslator *translator = new QTranslator();
    bool loaded = translator->load(filename, directory, searchDelimiters, suffix);
    if (loaded) {
        ScriptableTranslator *result = new ScriptableTranslator(translator, this);
        translator->setParent(result);
        translators.prepend(result);
        return result;
    } else {
        delete translator;
        return 0;
    }
}

bool ScriptableI18N::installApplicationTranslator(ScriptableTranslator *translator) {
    if (!translator) {
        return false;
    }
    translator->setParent(this);
    QTranslator *qtranslator = translator->translator();
    QCoreApplication::installTranslator(qtranslator);
    translator->setAutoRemoveApplicationTraslator(true);
    return true;
}

ScriptableTranslator *ScriptableI18N::installApplicationTranslator(const QString filename, const QString directory, const QString searchDelimiters, const QString suffix) {
    QTranslator *translator = new QTranslator();
    bool loaded = translator->load(filename, directory, searchDelimiters, suffix);
    if (loaded) {
        ScriptableTranslator *result = new ScriptableTranslator(translator, this);
        translator->setParent(result);
        QCoreApplication::installTranslator(translator);
        result->setAutoRemoveApplicationTraslator(true);
        return result;
    } else {
        delete translator;
        return 0;
    }
}

bool ScriptableI18N::installPermanentApplicationTranslator(ScriptableTranslator *translator) {
    if (!translator) {
        return false;
    }
    QTranslator *qtranslator = translator->translator();
    qtranslator->setParent(QCoreApplication::instance());
    QCoreApplication::installTranslator(qtranslator);
    return true;
}

ScriptableTranslator *ScriptableI18N::installPermanentApplicationTranslator(const QString filename, const QString directory, const QString searchDelimiters, const QString suffix) {
    QTranslator *translator = new QTranslator();
    bool loaded = translator->load(filename, directory, searchDelimiters, suffix);
    if (loaded) {
        ScriptableTranslator *result = new ScriptableTranslator(translator);
        translator->setParent(QCoreApplication::instance());
        QCoreApplication::installTranslator(translator);
        return result;
    } else {
        delete translator;
        return 0;
    }
}

void ScriptableI18N::removeTranslator(ScriptableTranslator *translator) {
    if (!translator) {
        return;
    }

    if (translators.removeAll(translator)) {
        QCoreApplication::removeTranslator(translator->translator());
    }
    translator->setParent(0);
    translator->setAutoRemoveApplicationTraslator(false);
}

void ScriptableI18N::installApplicationTranslatorFunctions(const QScriptValue &object) {
    /*
     * Bug: qsTr doesn't work in global scope
     * Fixed in Qt 4.6.3
     * See: http://bugreports.qt.nokia.com/browse/QTBUG-9775
     */
    engine()->installTranslatorFunctions(object);
}

static void replacePercentN(QString *result, int n)
{
    if (n >= 0) {
        int percentPos = 0;
        int len = 0;
        while ((percentPos = result->indexOf(QLatin1Char('%'), percentPos + len)) != -1) {
            len = 1;
            QString fmt;
            if (result->at(percentPos + len) == QLatin1Char('L')) {
                ++len;
                fmt = QLatin1String("%L1");
            } else {
                fmt = QLatin1String("%1");
            }
            if (result->at(percentPos + len) == QLatin1Char('n')) {
                fmt = fmt.arg(n);
                ++len;
                result->replace(percentPos, len, fmt);
                len = fmt.length();
            }
        }
    }
}

QString ScriptableI18N::translate (QString context, QString sourceText, QString disambiguation, QCoreApplication::Encoding encoding, int n) {
    QString result;

    if (sourceText.isEmpty()) {
        return result;
    }

    if (!translators.isEmpty()) {
        QList<ScriptableTranslator*>::ConstIterator it;
        ScriptableTranslator *translationFile;
        for (it = translators.constBegin(); it != translators.constEnd(); ++it) {
            translationFile = *it;
            result = translationFile->translator()->translate(context.toLatin1().constData(),
                                                              sourceText.toLatin1().constData(),
                                                              disambiguation.toLatin1().constData(),
                                                              n);
            if (!result.isEmpty())
                break;
        }
    } else {
        return QCoreApplication::translate(context.toLatin1().constData(),
                                           sourceText.toLatin1().constData(),
                                           disambiguation.toLatin1().constData(),
                                           encoding,
                                           n);
    }

    if (result.isEmpty()) {
        return QCoreApplication::translate(context.toLatin1().constData(),
                                           sourceText.toLatin1().constData(),
                                           disambiguation.toLatin1().constData(),
                                           encoding,
                                           n);
    } else {
        replacePercentN(&result, n);
        return result;
    }
}

static QScriptValue functionQsTranslate(QScriptContext *context, QScriptEngine *engine) {
    QString result;
    int argumentCount = context->argumentCount();
    if (argumentCount < 2) {
        context->throwError(QString::fromLatin1("qsTranslate() requires at least two arguments"));
        return engine->undefinedValue();
    }
    if (!context->argument(0).isString()) {
        context->throwError(QString::fromLatin1("qsTranslate(): first argument (context) must be a string"));
        return engine->undefinedValue();
    }
    if (!context->argument(1).isString()) {
        context->throwError(QString::fromLatin1("qsTranslate(): second argument (text) must be a string"));
        return engine->undefinedValue();
    }
    if ((argumentCount > 2) && !context->argument(2).isString()) {
        context->throwError(QString::fromLatin1("qsTranslate(): third argument (comment) must be a string"));
        return engine->undefinedValue();
    }
    if ((argumentCount > 3) && !context->argument(3).isString()) {
        context->throwError(QString::fromLatin1("qsTranslate(): fourth argument (encoding) must be a string"));
        return engine->undefinedValue();
    }
    if ((argumentCount > 4) && !context->argument(4).isNumber()) {
        context->throwError(QString::fromLatin1("qsTranslate(): fifth argument (n) must be a number"));
        return engine->undefinedValue();
    }

    QString text = context->argument(1).toString();
#ifndef QT_NO_QOBJECT
    QString contextName = context->argument(0).toString();

    QString comment;
    if (argumentCount > 2) {
        comment = context->argument(2).toString();
    }
    QCoreApplication::Encoding encoding = QCoreApplication::CodecForTr;
    if (argumentCount > 3) {
        QString encStr = context->argument(3).toString();
        if (encStr == QString::fromLatin1("CodecForTr")) {
            encoding = QCoreApplication::CodecForTr;
        } else if (encStr == QString::fromLatin1("UnicodeUTF8")) {
            encoding = QCoreApplication::UnicodeUTF8;
        } else {
            context->throwError(QString::fromLatin1("qsTranslate(): invalid encoding '%0'").arg(encStr));
            return engine->undefinedValue();
        }
    }

    int n = -1;
    if (argumentCount > 4) {
        n = context->argument(4).toInt32();
    }

    QScriptValue calleeData = context->callee().data();
    ScriptableI18N *i18n = qobject_cast<ScriptableI18N*>(calleeData.toQObject());
    result = i18n->translate(contextName, text, comment, encoding, n);
#else
    result = text;
#endif

    return engine->toScriptValue(result);
}

static QScriptValue functionQsTranslateNoOp(QScriptContext *context, QScriptEngine *engine) {
    if (context->argumentCount() < 2) {
        return engine->undefinedValue();
    }
    return context->argument(1);
}

static QScriptValue functionQsTr(QScriptContext *context, QScriptEngine *engine) {
    QString result;
    int argumentCount = context->argumentCount();
    if (argumentCount < 1) {
        context->throwError(QString::fromLatin1("qsTr() requires at least one argument"));
        return engine->undefinedValue();
    }
    if (!context->argument(0).isString()) {
        context->throwError(QString::fromLatin1("qsTr(): first argument (text) must be a string"));
        return engine->undefinedValue();
    }
    if ((argumentCount > 1) && !context->argument(1).isString()) {
        context->throwError(QString::fromLatin1("qsTr(): second argument (comment) must be a string"));
        return engine->undefinedValue();
    }
    if ((argumentCount > 2) && !context->argument(2).isNumber()) {
        context->throwError(QString::fromLatin1("qsTr(): third argument (n) must be a number"));
        return engine->undefinedValue();
    }

    QString text = context->argument(0).toString();
#ifndef QT_NO_QOBJECT
    QScriptValue calleeData = context->callee().data();
    ScriptableI18N *i18n = qobject_cast<ScriptableI18N*>(calleeData.toQObject());

    QString contextName = i18n->getCurrentContext(context);

    QString comment;
    if (argumentCount > 1) {
        comment = context->argument(1).toString();
    }
    int n = -1;
    if (argumentCount > 2) {
        n = context->argument(2).toInt32();
    }

    result = i18n->translate(contextName, text, comment, QCoreApplication::CodecForTr, n);
#else
    result = text;
#endif

    return engine->toScriptValue(result);
}

static QScriptValue functionQsTrNoOp(QScriptContext *context, QScriptEngine *engine) {
    if (context->argumentCount() < 1) {
        return engine->undefinedValue();
    }
    return context->argument(0);
}

static QScriptValue functionQsTrId(QScriptContext *context, QScriptEngine *engine) {
    QString result;
    int argumentCount = context->argumentCount();
    if (argumentCount < 1) {
        context->throwError(QString::fromLatin1("qsTrId() requires at least one argument"));
        return engine->undefinedValue();
    }
    if (!context->argument(0).isString()) {
        context->throwError(QString::fromLatin1("qsTrId(): first argument (id) must be a string"));
        return engine->undefinedValue();
    }
    if ((argumentCount > 1) && !context->argument(1).isNumber()) {
        context->throwError(QString::fromLatin1("qsTrId(): second argument (n) must be a number"));
        return engine->undefinedValue();
    }
    QString id = context->argument(0).toString();
    int n = -1;
    if (argumentCount > 1) {
        n = context->argument(1).toInt32();
    }

    QScriptValue calleeData = context->callee().data();
    ScriptableI18N *i18n = qobject_cast<ScriptableI18N*>(calleeData.toQObject());
    result = i18n->translate(QString(), id, QString(), QCoreApplication::UnicodeUTF8, n);
    return engine->toScriptValue(result);
}

static QScriptValue functionQsTrIdNoOp(QScriptContext *context, QScriptEngine *engine) {
    if (context->argumentCount() < 1) {
        return engine->undefinedValue();
    }
    return context->argument(0);
}

// alternative for install String::arg function, don't work, see installTranslatorFunctions
//static QScriptValue functionArg(QScriptContext *context, QScriptEngine *engine) {
//    QString value = context->thisObject().toString();
//    QScriptValue arg;
//    QString result;
//
//    if (context->argumentCount() != 1) {
//        arg = context->argument(0);
//    }
//
//    if (arg.isString()) {
//        result = value.arg(arg.toString());
//    } else if (arg.isNumber()) {
//        result = value.arg(arg.toNumber());
//    }
//
//    return engine->toScriptValue(result);
//}

void ScriptableI18N::installTranslatorFunctions(const QScriptValue &object, QScriptEngine &engine) {
    QScriptValue obj = object;
    if (object.isNull() || !object.isObject()) {
        obj = engine.globalObject();
    }
    QScriptValue data = engine.newQObject(this);

    // needed for install String::arg function
    engine.installTranslatorFunctions(obj);

    QScriptValue qsTranslateFunction = engine.newFunction(functionQsTranslate);
    qsTranslateFunction.setData(data);
    obj.setProperty(QString::fromLatin1("qsTranslate"), qsTranslateFunction);

    QScriptValue qsTranslateNoOpFunction = engine.newFunction(functionQsTranslateNoOp);
    qsTranslateNoOpFunction.setData(data);
    obj.setProperty(QString::fromLatin1("QT_TRANSLATE_NOOP"), qsTranslateNoOpFunction);

    QScriptValue qsTrFunction = engine.newFunction(functionQsTr);
    qsTrFunction.setData(data);
    obj.setProperty(QString::fromLatin1("qsTr"), qsTrFunction);

    QScriptValue qsTrNoOpFunction = engine.newFunction(functionQsTrNoOp);
    qsTrNoOpFunction.setData(data);
    obj.setProperty(QString::fromLatin1("QT_TR_NOOP"), qsTrNoOpFunction);

    QScriptValue qsTrIdFunction = engine.newFunction(functionQsTrId);
    qsTrIdFunction.setData(data);
    obj.setProperty(QString::fromLatin1("qsTrId"), qsTrIdFunction);

    QScriptValue qsTrIdNoOpFunction = engine.newFunction(functionQsTrIdNoOp);
    qsTrIdNoOpFunction.setData(data);
    obj.setProperty(QString::fromLatin1("QT_TRID_NOOP"), qsTrIdNoOpFunction);

    // alternative for install String::arg function, don't work
//    QScriptValue argFunction = engine.newFunction(functionArg);
//    obj = engine.globalObject().property(QString::fromLatin1("String")).prototype();
//    obj.setProperty(QString::fromLatin1("arg"), argFunction);
}

void ScriptableI18N::installTranslatorFunctions(const QScriptValue &object) {
    installTranslatorFunctions(object, *engine());
}

QString ScriptableI18N::languageToString(QString language) const {
    int index = ScriptableLocale::languageIndex(language);
    if (index >= 0) {
        return QLocale::languageToString((QLocale::Language)index);
    } else {
        return QString();
    }
}

QString ScriptableI18N::countryToString(QString country) const {
    int index = ScriptableLocale::countryIndex(country);
    if (index >= 0) {
        return QLocale::countryToString((QLocale::Country)index);
    } else {
        return QString();
    }
}

ScriptableLocale *ScriptableI18N::defaultLocale() const {
    return new ScriptableLocale(QLocale());
}

void ScriptableI18N::setDefaultLocale(const ScriptableLocale* locale) {
    QLocale::setDefault(locale->locale());
}

ScriptableLocale *ScriptableI18N::cLocale() const {
    return new ScriptableLocale(QLocale::c());
}

ScriptableLocale *ScriptableI18N::systemLocale() const {
    return new ScriptableLocale(QLocale::system());
}

ScriptableLocale *ScriptableI18N::localeByName(QString name) {
    return new ScriptableLocale(QLocale(name));
}

ScriptableLocale *ScriptableI18N::locale(QString language, QString country) {
    int indexl = ScriptableLocale::languageIndex(language);
    int indexc = ScriptableLocale::countryIndex(country);
    if (indexl < 0 || indexl < 0) {
        return 0;
    }
    return new ScriptableLocale(QLocale((QLocale::Language)indexl, (QLocale::Country)indexc));
}

QList<QString> countriesForLanguage(QString lang) {
    int index = ScriptableLocale::languageIndex(lang);
    if (index < 0) {
        return QList<QString>();
    }
    QList<QString> result;
    QList<QLocale::Country> tmp = QLocale::countriesForLanguage((QLocale::Language)index);

    const QMetaObject &mo = QLocale::staticMetaObject;
    int indexe = mo.indexOfEnumerator("Country");
    QMetaEnum metaEnum = mo.enumerator(indexe);

    foreach(QLocale::Country c, tmp) {
        result << QString::fromLatin1(metaEnum.valueToKey(c));
    }

    return result;
}
