Script Report
Visit: http://code.google.com/p/scriptreport/

Script Report is a script report engine for create print report using
html and javascript.

The Script Report Engine uses QtScript module for run a javascript
for generate HTML, one HTML document per document sections (header, 
footer, content, etc.), and render these HTML using the Qt 
QTextDocument, in consequence all Qt HTML subset is supported.

This product uses software developed by:
* The Nokia Qt Project, http://qt.nokia.com/

For source availability and license information see copying.txt.

How build it:

For compile the application run:

$> qmake -recursive scriptreport.pro
$> make

The compilation result will be in the compiled folder.

How build the documentation:

For generate the documentation open the doc folder and run:

$> qdoc3 scriptreportengine.qdocconf

The generated documentation will be in the doc/html folder, for see it open the index.html file. 

For extract translatable text from the source code open the editor folder and run:

$> lupdate editor.pro

