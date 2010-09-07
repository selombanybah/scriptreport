TEMPLATE = subdirs

#http://www.archivum.info/qt-interest@trolltech.com/2007-03/00639/Re:-Using-qmake-subdirs,-ordered,-AND-dependencies.html
SUBDIRS = sub_engine \
          sub_editor \
          sub_srsql \
          sub_tools 

sub_engine.subdir = engine
sub_editor.subdir = editor
sub_editor.depends = sub_engine
sub_srsql.subdir = srsql
sub_tools.subdir = tools
sub_tools.depends = sub_engine
