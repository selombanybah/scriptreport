/*
 * Copyright 2010 and beyond, Juan Luis Paz
 *
 * This file is part of Script Report.
 *
 * Script Report is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Script Report is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Script Report.  If not, see <http://www.gnu.org/licenses/>.
 */

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
