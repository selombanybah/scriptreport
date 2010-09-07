#include <QtGui/QApplication>

#include <consoleshell.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConsoleShell *s = new ConsoleShell(&a);
    int returnCode;
    if (s->init(returnCode)) {
        return returnCode;
    }
    return a.exec();
}
