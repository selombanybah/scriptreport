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

#ifndef SCRIPTABLEPAPERMARGINS_H
#define SCRIPTABLEPAPERMARGINS_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtScript/QScriptable>

class QPrinter;

class ScriptablePaperMargins : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QString unit READ unitByName WRITE setUnitByName)
    Q_PROPERTY(qreal top READ top WRITE setTop)
    Q_PROPERTY(qreal right READ right WRITE setRight)
    Q_PROPERTY(qreal bottom READ bottom WRITE setBottom)
    Q_PROPERTY(qreal left READ left WRITE setLeft)
    Q_ENUMS(Unit)

    // unit property by name for validate and throw an error
public:
    enum Unit {
        Millimeter,
        Point,
        Inch,
        Pica,
        Didot,
        Cicero,
        Centimeter,
        mm = Millimeter,
        cm = Centimeter,
        pt = Point,
        in = Inch,
        pc = Pica
    };

    explicit ScriptablePaperMargins(QObject *parent = 0);

    Unit unit() const;
    void setUnit(Unit unit);

    QString unitByName() const;
    void setUnitByName(QString unit);

    qreal top() const;
    void setTop(qreal top);

    qreal right() const;
    void setRight(qreal right);

    qreal bottom() const;
    void setBottom(qreal bottom);

    qreal left() const;
    void setLeft(qreal left);

    Q_INVOKABLE void setMargins(qreal top, qreal right, qreal bottom, qreal left, QString unit);
    Q_INVOKABLE void updateUnit(QString unit);

    void loadConfigurationFrom(QPrinter &printer);
    void applyConfigurationTo(QPrinter &printer);

private:
    qreal m_top;
    qreal m_right;
    qreal m_bottom;
    qreal m_left;
    Unit m_unit;
    bool m_isMarginsChanged;

};

Q_DECLARE_METATYPE(ScriptablePaperMargins*)

#endif // SCRIPTABLEPAPERMARGINS_H
