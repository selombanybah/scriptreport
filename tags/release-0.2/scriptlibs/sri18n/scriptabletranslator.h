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

#ifndef SCRIPTABLETRANSLATOR_H
#define SCRIPTABLETRANSLATOR_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QTranslator>
#include <QtScript/QScriptable>

class ScriptableTranslator : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty)

public:
    explicit ScriptableTranslator(QTranslator *translator, QObject *parent = 0);
    ~ScriptableTranslator();

    Q_INVOKABLE QString translate(const QString context, const QString sourceText, const QString disambiguation = QString()) const;
    Q_INVOKABLE QString translate(const QString context, const QString sourceText, const QString disambiguation, int n) const;

    bool isEmpty() const;

    QTranslator *translator() const;

    bool autoRemoveApplicationTraslator() const;
    void setAutoRemoveApplicationTraslator(bool autoRemove);
private:
    QTranslator *m_translator;
    bool m_autoRemoveApplicationTraslator;
};

Q_DECLARE_METATYPE(ScriptableTranslator*)

#endif // SCRIPTABLETRANSLATOR_H
