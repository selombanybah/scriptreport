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

#include "scriptabletranslator.h"

#include <QtCore/QCoreApplication>

ScriptableTranslator::ScriptableTranslator(QTranslator *translator, QObject *parent) :
    QObject(parent), QScriptable(),
    m_translator(translator),
    m_autoRemoveApplicationTraslator(false)
{
}

ScriptableTranslator::~ScriptableTranslator() {
    if (m_autoRemoveApplicationTraslator) {
        QCoreApplication::removeTranslator(m_translator);
    }
}

QString ScriptableTranslator::translate(const QString context, const QString sourceText, const QString disambiguation) const {
    if (disambiguation.isEmpty()) {
        return m_translator->translate(context.toLatin1().constData(), sourceText.toLatin1().constData());
    } else {
        return m_translator->translate(context.toLatin1().constData(), sourceText.toLatin1().constData(), disambiguation.toLatin1().constData());
    }
}

QString ScriptableTranslator::translate(const QString context, const QString sourceText, const QString disambiguation, int n) const {
    return m_translator->translate(context.toLatin1().constData(), sourceText.toLatin1().constData(), disambiguation.toLatin1().constData(), n);
}

bool ScriptableTranslator::isEmpty() const {
    return m_translator->isEmpty();
}

QTranslator *ScriptableTranslator::translator() const {
    return m_translator;
}

bool ScriptableTranslator::autoRemoveApplicationTraslator() const {
    return m_autoRemoveApplicationTraslator;
}

void ScriptableTranslator::setAutoRemoveApplicationTraslator(bool autoRemove) {
    m_autoRemoveApplicationTraslator = autoRemove;
}
