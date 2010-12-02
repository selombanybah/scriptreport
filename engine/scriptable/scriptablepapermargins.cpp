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

#include "scriptablepapermargins.h"

#include <QtCore/QMetaEnum>
#include <QtGui/QPrinter>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValue>

/*
 * Statics
 */

static int getUnitValue(QString unit) {
    const QMetaObject &mo = ScriptablePaperMargins::staticMetaObject;
    int index = mo.indexOfEnumerator("Unit");
    QMetaEnum metaEnum = mo.enumerator(index);

    return metaEnum.keyToValue(unit.toLatin1().constData());
}

static qreal unitPointConversionFactor(ScriptablePaperMargins::Unit unit) {
    switch(unit) {
    case ScriptablePaperMargins::Millimeter:
        return 2.83464566929;
    case ScriptablePaperMargins::Centimeter:
        return 28.3464566929;
    case ScriptablePaperMargins::Point:
        return 1.0;
    case ScriptablePaperMargins::Inch:
        return 72.0;
    case ScriptablePaperMargins::Pica:
        return 12;
    case ScriptablePaperMargins::Didot:
        return 1.065826771;
    case ScriptablePaperMargins::Cicero:
        return 12.789921252;
    }
    return 1.0;
}

static QPrinter::Unit toQPrinter(ScriptablePaperMargins::Unit unit) {
    switch(unit) {
    case ScriptablePaperMargins::Centimeter:
        qCritical("Centimeter is not a QPrinter unit");
    case ScriptablePaperMargins::Millimeter:
        return QPrinter::Millimeter;
    case ScriptablePaperMargins::Point:
        return QPrinter::Point;
    case ScriptablePaperMargins::Inch:
        return QPrinter::Inch;
    case ScriptablePaperMargins::Pica:
        return QPrinter::Pica;
    case ScriptablePaperMargins::Didot:
        return QPrinter::Didot;
    case ScriptablePaperMargins::Cicero:
        return QPrinter::Cicero;
    }
    return QPrinter::Point;
}

/*
 * Class
 */

ScriptablePaperMargins::ScriptablePaperMargins(QObject *parent) :
    QObject(parent), QScriptable()
{
}

ScriptablePaperMargins::Unit ScriptablePaperMargins::unit() const {
    return m_unit;
}

void ScriptablePaperMargins::setUnit(ScriptablePaperMargins::Unit unit) {
    if (unit >= Millimeter && unit <= Centimeter) {
        m_unit = unit;
        m_isMarginsChanged = true;
    } else {
        QString arg = argument(0).toString();
        context()->throwError(tr("%1 is not a valid unit.").arg(arg));
    }
}

QString ScriptablePaperMargins::unitByName() const {
    const QMetaObject &mo = ScriptablePaperMargins::staticMetaObject;
    int index = mo.indexOfEnumerator("Unit");
    QMetaEnum metaEnum = mo.enumerator(index);

    return QString::fromLatin1(metaEnum.valueToKey(m_unit));
}

void ScriptablePaperMargins::setUnitByName(QString unit) {
    int u = getUnitValue(unit);
    if (u == -1) {
        context()->throwError(tr("%1 is not a valid unit.").arg(unit));
    } else {
        m_unit = (Unit) u;
        m_isMarginsChanged = true;
    }
}

qreal ScriptablePaperMargins::top() const {
    return m_top;
}

void ScriptablePaperMargins::setTop(qreal top) {
    m_top = top;
    m_isMarginsChanged = true;
}

qreal ScriptablePaperMargins::right() const {
    return m_right;
}

void ScriptablePaperMargins::setRight(qreal right) {
    m_right = right;
    m_isMarginsChanged = true;
}

qreal ScriptablePaperMargins::bottom() const {
    return m_bottom;
}

void ScriptablePaperMargins::setBottom(qreal bottom) {
    m_bottom = bottom;
    m_isMarginsChanged = true;
}

qreal ScriptablePaperMargins::left() const {
    return m_left;
}

void ScriptablePaperMargins::setLeft(qreal left) {
    m_left = left;
    m_isMarginsChanged = true;
}

void ScriptablePaperMargins::setMargins(qreal top, qreal right, qreal bottom, qreal left, QString unit) {
    // QPrinter not use css margins parameter order
    int i = getUnitValue(unit);
    if (i == -1) {
        return;
    }
    m_unit = (Unit) i;
    m_top = top;
    m_right = right;
    m_bottom = bottom;
    m_left = left;
    m_isMarginsChanged = true;
}

void ScriptablePaperMargins::updateUnit(QString unit) {
    int i = getUnitValue(unit);
    if (i == -1) {
        return;
    }
    Unit u = (Unit) i;
    // to point unit
    qreal factor = unitPointConversionFactor(m_unit);
    m_top    = m_top    * factor;
    m_right  = m_right  * factor;
    m_bottom = m_bottom * factor;
    m_left   = m_left   * factor;
    // to new unit
    factor = unitPointConversionFactor(u);
    m_top    = m_top    / factor;
    m_right  = m_right  / factor;
    m_bottom = m_bottom / factor;
    m_left   = m_left   / factor;
    m_unit   = u;
}

void ScriptablePaperMargins::loadConfigurationFrom(QPrinter &printer) {
    // QPrinter not use css margins parameter order
    printer.getPageMargins(&m_left, &m_top, &m_right, &m_bottom, QPrinter::Point);
    m_unit = Point;
    m_isMarginsChanged = false;
}

void ScriptablePaperMargins::applyConfigurationTo(QPrinter &printer) {
    // QPrinter not use css margins parameter order
    if (m_isMarginsChanged) {
        if (m_unit == Centimeter) {
            printer.setPageMargins(m_left * 10, m_top * 10, m_right * 10, m_bottom * 10, QPrinter::Millimeter);
        } else {
            printer.setPageMargins(m_left, m_top, m_right, m_bottom, toQPrinter(m_unit));
        }
    }
}
