#include <QtGui/QApplication>

#include "scriptreporttool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScriptReportTool *s = new ScriptReportTool(&a);
    int returnCode;
    if (s->init(returnCode)) {
        return returnCode;
    }
    return a.exec();
}
