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

#include "scriptablepaper.h"

#include <QtCore/QMetaEnum>
#include <QtGui/QPrinter>
#include <QtScript/QScriptEngine>

/*
 * Statics
 */

ScriptablePaper::Orientation fromQPrinter(QPrinter::Orientation orientation) {
    switch(orientation) {
    case QPrinter::Portrait:
        return ScriptablePaper::Portrait;
    case QPrinter::Landscape:
        return ScriptablePaper::Landscape;
    }
    return ScriptablePaper::Portrait;
}

QPrinter::Orientation toQPrinter(ScriptablePaper::Orientation orientation) {
    switch(orientation) {
    case ScriptablePaper::Portrait:
        return QPrinter::Portrait;
    case ScriptablePaper::Landscape:
        return QPrinter::Landscape;
    }
    return QPrinter::Portrait;
}

QScriptValue paperSizeToScriptValue(QScriptEngine *engine, ScriptablePaperSize* const &in) {
    return engine->newQObject(in, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
}

void paperSizeFromScriptValue(const QScriptValue &object, ScriptablePaperSize* &out) {
    out = qobject_cast<ScriptablePaperSize*>(object.toQObject());
}

QScriptValue paperMarginsToScriptValue(QScriptEngine *engine, ScriptablePaperMargins* const &in) {
    return engine->newQObject(in, QScriptEngine::QtOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
}

void paperMarginsFromScriptValue(const QScriptValue &object, ScriptablePaperMargins* &out) {
    out = qobject_cast<ScriptablePaperMargins*>(object.toQObject());
}

/*
 * Class
 */

ScriptablePaper::ScriptablePaper(QObject *parent) :
    QObject(parent), QScriptable()
{
    m_size = new ScriptablePaperSize(this);
    m_margins = new ScriptablePaperMargins(this);
}

ScriptablePaper::Orientation ScriptablePaper::orientation() const {
    return m_orientation;
}

void ScriptablePaper::setOrientation(ScriptablePaper::Orientation orientation) {
    if (orientation >= Portrait && orientation <= Landscape) {
        m_orientation = orientation;
        m_isOrientationChanged = true;
    } else {
        QString arg = argument(0).toString();
        context()->throwError(tr("%1 is not a valid orientation.").arg(arg));
    }
}

QString ScriptablePaper::orientationByName() const {
    const QMetaObject &mo = ScriptablePaper::staticMetaObject;
    int index = mo.indexOfEnumerator("Orientation");
    QMetaEnum metaEnum = mo.enumerator(index);

    return QString::fromLatin1(metaEnum.valueToKey(m_orientation));
}

void ScriptablePaper::setOrientationByName(QString orientation) {
    const QMetaObject &mo = ScriptablePaper::staticMetaObject;
    int index = mo.indexOfEnumerator("Orientation");
    QMetaEnum metaEnum = mo.enumerator(index);

    int o = metaEnum.keyToValue(orientation.toLatin1().constData());
    if (o == -1) {
        context()->throwError(tr("%1 is not a valid orientation.").arg(orientation));
    } else {
        m_orientation = (Orientation) o;
        m_isOrientationChanged = true;
    }
}

ScriptablePaperSize* ScriptablePaper::size() const {
    return m_size;
}

ScriptablePaperMargins* ScriptablePaper::margins() const {
    return m_margins;
}

void ScriptablePaper::loadConfigurationFrom(QPrinter &printer) {
    m_orientation = fromQPrinter(printer.orientation());
    m_isOrientationChanged = false;
    m_size->loadConfigurationFrom(printer);
    m_margins->loadConfigurationFrom(printer);
}

void ScriptablePaper::applyConfigurationTo(QPrinter &printer) {
    if (m_isOrientationChanged) {
        printer.setOrientation(toQPrinter(m_orientation));
    }
    m_size->applyConfigurationTo(printer);
    m_margins->applyConfigurationTo(printer);
}

void ScriptablePaper::initEngine(QScriptEngine &engine) {
    qScriptRegisterMetaType(&engine, paperSizeToScriptValue, paperSizeFromScriptValue);
    qScriptRegisterMetaType(&engine, paperMarginsToScriptValue, paperMarginsFromScriptValue);
}
