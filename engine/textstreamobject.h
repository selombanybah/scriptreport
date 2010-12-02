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

#ifndef TEXTSTREAMOBJECT_H
#define TEXTSTREAMOBJECT_H

#include "scriptreportengine_global.h"

#include <QtCore/QObject>
#include <QtCore/QTextStream>

class TextStreamObjectPrivate;

class SCRIPTREPORTENGINE_EXPORT TextStreamObject : public QObject
{
Q_OBJECT
public:
    TextStreamObject(QString name, QIODevice::OpenMode defaultStreamMode, QObject* parent = 0);
    ~TextStreamObject();

//    void reset();

    QTextStream* stream() const;
    void setStream(QTextStream* textStream, bool forDelete = false);

    QString text() const;
    void setText(QString text);

    QString name() const;
    QIODevice::OpenMode defaultStreamMode() const;
    bool isDeleteStreamEmabled() const;

private:
    TextStreamObjectPrivate *d;
};

#endif // TEXTSTREAMOBJECT_H
