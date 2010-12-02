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

#ifndef SCRIPTREPORTENGINE_H
#define SCRIPTREPORTENGINE_H

#include "scriptreportengine_global.h"

#include <QtCore/QObject>
#include "scriptreport.h"

class QPrinter;
class QScriptEngine;

class ScriptReportEnginePrivate;

class SCRIPTREPORTENGINE_EXPORT ScriptReportEngine
{
public:
    ScriptReportEngine();
    virtual ~ScriptReportEngine();

    virtual void loadPrintConfiguration(ScriptReport *scriptReport, QPrinter *printer);
    virtual void print(ScriptReport *scriptReport, QPrinter *printer);
    virtual void initEngine(ScriptReport *scriptReport, QScriptEngine *engine);

private:
    ScriptReportEnginePrivate *d;
};

#endif // SCRIPTREPORTENGINE_H
