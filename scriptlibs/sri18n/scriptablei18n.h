#ifndef SCRIPTABLEI18N_H
#define SCRIPTABLEI18N_H

#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>

#include "scriptabletranslator.h"
#include "scriptablelocale.h"

class ScriptableI18N : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QString currentContext READ currentContext)
    Q_PROPERTY(ScriptableLocale* defaultLocale READ defaultLocale WRITE setDefaultLocale)
    Q_PROPERTY(ScriptableLocale* cLocale READ cLocale)
    Q_PROPERTY(ScriptableLocale* systemLocale READ systemLocale)

public:
    explicit ScriptableI18N(QObject *parent = 0);

    QString currentContext();

    Q_INVOKABLE ScriptableTranslator *openTranslator(const QString filename, const QString directory = QString(), const QString searchDelimiters = QString(), const QString suffix = QString());

    Q_INVOKABLE bool installTranslator(ScriptableTranslator *translator);
    Q_INVOKABLE ScriptableTranslator *installTranslator(const QString filename, const QString directory = QString(), const QString searchDelimiters = QString(), const QString suffix = QString());
    Q_INVOKABLE bool installApplicationTranslator(ScriptableTranslator *translator);
    Q_INVOKABLE ScriptableTranslator *installApplicationTranslator(const QString filename, const QString directory = QString(), const QString searchDelimiters = QString(), const QString suffix = QString());
    Q_INVOKABLE bool installPermanentApplicationTranslator(ScriptableTranslator *translator);
    Q_INVOKABLE ScriptableTranslator *installPermanentApplicationTranslator(const QString filename, const QString directory = QString(), const QString searchDelimiters = QString(), const QString suffix = QString());
    Q_INVOKABLE void removeTranslator(ScriptableTranslator *translator);

    Q_INVOKABLE void installApplicationTranslatorFunctions(const QScriptValue &object = QScriptValue());
    void installTranslatorFunctions(const QScriptValue &object, QScriptEngine &engine);
    Q_INVOKABLE void installTranslatorFunctions(const QScriptValue &object = QScriptValue());

    QString translate (QString context, QString sourceText, QString disambiguation = QString(), QCoreApplication::Encoding encoding = QCoreApplication::CodecForTr, int n = -1);

    QString getCurrentContext(QScriptContext *context);

    Q_INVOKABLE QString languageToString(QString language) const;
    Q_INVOKABLE QString countryToString(QString country) const;

    ScriptableLocale *defaultLocale() const;
    void setDefaultLocale(const ScriptableLocale* locale);
    ScriptableLocale *cLocale() const;
    ScriptableLocale *systemLocale() const;

    Q_INVOKABLE ScriptableLocale *localeByName(QString name);
    Q_INVOKABLE ScriptableLocale *locale(QString language, QString country = QLatin1String("AnyCountry"));

    Q_INVOKABLE QList<QString> countriesForLanguage(QString lang);

private:
    QList<ScriptableTranslator*> translators;
    QScriptContext *cachedTranslationContext;
    QString cachedTranslationContextName;
};

#endif // SCRIPTABLEI18N_H
