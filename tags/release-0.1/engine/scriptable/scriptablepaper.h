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

#ifndef SCRIPTABLEPAPER_H
#define SCRIPTABLEPAPER_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtScript/QScriptable>

#include "scriptablepapersize.h"
#include "scriptablepapermargins.h"

class QPrinter;
class QScriptEngine;

class ScriptablePaper : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QString orientation READ orientationByName WRITE setOrientationByName)
    Q_PROPERTY(ScriptablePaperSize* size READ size)
    Q_PROPERTY(ScriptablePaperMargins* margins READ margins)
    Q_ENUMS(Orientation)

    // unit property by name for validate and throw an error
public:
    enum Orientation {Portrait, Landscape};

    explicit ScriptablePaper(QObject *parent = 0);

    Orientation orientation() const;
    void setOrientation(Orientation orientation);

    QString orientationByName() const;
    void setOrientationByName(QString orientation);

    ScriptablePaperSize* size() const;
    ScriptablePaperMargins* margins() const;

    void loadConfigurationFrom(QPrinter &printer);
    void applyConfigurationTo(QPrinter &printer);
    void initEngine(QScriptEngine &engine);

private:
    Orientation m_orientation;
    ScriptablePaperSize* m_size;
    ScriptablePaperMargins* m_margins;
    bool m_isOrientationChanged;
};

Q_DECLARE_METATYPE(ScriptablePaper*)

#endif // SCRIPTABLEPAPER_H
