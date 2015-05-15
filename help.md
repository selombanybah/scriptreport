# Content #

The zip file contents this relevant folders:

  * **compiled**: contains the compiled files when the project is compiled.
    * **script**: contains the compiled files of Qt QScript plugins for the javascript libraries when the project is compiled.
  * **editor**: contains the script report editor source code.
  * **examples** contains some examples.
    * **music**: contains a very complete example.
    * **tests**: contains another examples for test propose.
  * **scriptlibs**: contains the source code of the javascript libraries.
    * **sri18n**: contains the source code of the internationalization javascript library.
    * **srsql**: contains the source code of the Sql javascript library for access to databases.
  * **doc**: contains the files for generate the documentation.
  * **engine**: contains the source code of the script report engine.
  * **includes**: contains the includes files for projects that use script report.
  * **tools**: contains some tools.
    * **scriptreporttool**: source code of the tool for run or generate the javascript of a report.
    * **srsh**: source code of a javascript console shell.
    * **srshcommon**: common source code of javascript console shell.
    * **srshqg**: source code of a javascript console shell with support to **Qt Gui module**.

And contains this relevant files:

  * **scriptreport.pro**: qmake project of script report.
  * **doc/scriptreportengine.qdocconf**: qdoc3 file for generate the documentation.

# Requirements #

For compile and run you need Nokia Qt 4.6 minimum.

# Supported platform #

All supported by Qt, like Windows, Linux and Mac OS.

# Generated compiled files #

The compiled files will be generated in the compiled folder, the exactly name depends of your platform.

  * **scriptreportengine**: library with the script report engine, required for run a report.
  * **scriptreporttool**: executable with a tool for run or generate the javascript of a report.
  * **script/sri18n**: **QScript** plugin with the internationalization javascript library.
  * **script/srsql**: **QScript** plugnin with the Sql javascript library for access to databases.
  * **srsh**: executable with a javascript console shell.
  * **scriptreporteditor**: executable with the script report editor.
  * **srshqg**: executable with a javascript console shell with support to **Qt Gui module**.

# How build it #

For compile the application run:

```
$> qmake -recursive scriptreport.pro
$> make
```

The compilation result will be in the `compiled` folder.

# How build the documentation #

For generate the documentation open the `doc` folder and run:

```
$> qdoc3 scriptreportengine.qdocconf
```

The generated documentation will be in the `doc/html` folder, for see it open the `index.html` file.

# How use it #

**Warning**: don't forget add the `scriptreportengine` library to your project folder and add the QScript plugins if you want to use them.

## Run a report ##
```
QPrinter printer;
ScriptReport sr("myReport.srt");
sr.print(&printer);
```

## Run a report showing a print dialog ##
```
QPrinter printer;
QPrintDialog printDialog(&printer);
if (printDialog.exec() == QDialog::Accepted) {
    ScriptReport sr("myReport.srt");
    sr.print(&printer);
}
```

## Run a report showing a print preview ##
```
ScriptReport *sr = new ScriptReport("myReport.srt");
QPrintPreviewDialog *p = new QPrintPreviewDialog();
connect(p, SIGNAL(paintRequested(QPrinter*)), sr, SLOT(print(QPrinter*)));
p->exec();
delete p;
delete sr;
```

## Examples ##

  * **Music report**: contains a very complete example about consult a database, construct the report html and add internationalization suport. The source code of the music example is music.srt (located in the folder `examples/music`), [click here for see the source code](http://scriptreport.googlecode.com/svn/files/examples/music/music.html), and [here for see the print result](http://scriptreport.googlecode.com/svn/files/examples/music/music.pdf).

  * **Current time**: an example using QWidget and QPixmap for draw images in the report. The source code of the current time example is currenttime.srt (located in the folder `examples`), [click here for see the source code](http://scriptreport.googlecode.com/svn/files/examples/currenttime.html), and [here for see the print result](http://scriptreport.googlecode.com/svn/files/examples/currenttime.pdf).

## Documentation ##

  * [API documentation](http://scriptreport.googlecode.com/svn/files/qdoc/index.html)
  * [Supported HTML subset](http://scriptreport.googlecode.com/svn/files/help%20tabs/Supported_HTML_Subset.html)
  * [JavaScript functions](http://scriptreport.googlecode.com/svn/files/help%20tabs/javascript_functions.html)