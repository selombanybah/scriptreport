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

#ifndef SCRIPTABLEPAPERSIZE_H
#define SCRIPTABLEPAPERSIZE_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtScript/QScriptable>

class QPrinter;

class ScriptablePaperSize : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QString size READ sizeByName WRITE setSizeByName)
    Q_PROPERTY(QString unit READ unitByName WRITE setUnitByName)
    Q_PROPERTY(qreal height READ height WRITE setHeight)
    Q_PROPERTY(qreal width READ width WRITE setWidth)
    Q_ENUMS(Size Unit)

    // size and unit properties by name for validate and throw an error

public:
    enum Size { A4, B5, Letter, Legal, Executive,
                A0, A1, A2, A3, A5, A6, A7, A8, A9, B0, B1,
                B10, B2, B3, B4, B6, B7, B8, B9, C5E, Comm10E,
                DLE, Folio, Ledger, Tabloid, Custom };

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

    explicit ScriptablePaperSize(QObject *parent = 0);

    Size size() const;
    void setSize(Size size);

    QString sizeByName() const;
    void setSizeByName(QString size);

    Unit unit() const;
    void setUnit(Unit unit);

    QString unitByName() const;
    void setUnitByName(QString unit);

    qreal height() /*const*/;
    void setHeight(qreal height);

    qreal width() /*const*/;
    void setWidth(qreal width);

    Q_INVOKABLE void setSize(qreal height, qreal width, QString unit);
    Q_INVOKABLE void updateUnit(QString unit);

    void loadConfigurationFrom(QPrinter &printer);
    void applyConfigurationTo(QPrinter &printer);

private:
    void updateHeightAndWidth();

private:
    Size m_size;
    Unit m_unit;
    qreal m_height;
    qreal m_width;
    bool m_isSizeChanged;
    bool m_isUpdateHeightAndWidthNeeded;
};

Q_DECLARE_METATYPE(ScriptablePaperSize*)

#endif // SCRIPTABLEPAPERSIZE_H
