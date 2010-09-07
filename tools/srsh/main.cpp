#include <QtCore/QCoreApplication>

#include <consoleshell.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ConsoleShell *s = new ConsoleShell(&a);
    int returnCode;
    if (s->init(returnCode)) {
        return returnCode;
    }
    return a.exec();
}
