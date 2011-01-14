#
# Copyright 2010 and beyond, Juan Luis Paz
#
# This file is part of Script Report.
#
# Script Report is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Script Report is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Script Report.  If not, see <http://www.gnu.org/licenses/>.
#

include(scriptreport.pri)

TEMPLATE = subdirs

SUBDIRS = sub_engine \
          sub_editor \
          sub_scriptlibs \
          sub_tools 

sub_engine.subdir = engine
sub_editor.subdir = editor
sub_editor.depends = sub_engine
sub_scriptlibs.subdir = scriptlibs
sub_tools.subdir = tools
sub_tools.depends = sub_engine
