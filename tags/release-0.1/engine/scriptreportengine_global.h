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

#ifndef SCRIPTREPORTENGINE_GLOBAL_H
#define SCRIPTREPORTENGINE_GLOBAL_H

#include <QtCore/qglobal.h>

#if !defined(SCRIPTREPORTENGINE_NOLIBRARY)

#   if defined(SCRIPTREPORTENGINE_LIBRARY)
#       define SCRIPTREPORTENGINE_EXPORT Q_DECL_EXPORT
#   else
#       define SCRIPTREPORTENGINE_EXPORT Q_DECL_IMPORT
#   endif

#else
#  define SCRIPTREPORTENGINE_EXPORT
#endif

#endif // ENGINE_GLOBAL_H
