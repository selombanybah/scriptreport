#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(MainWindow::tr("Script Report Editor"));
    a.setApplicationVersion(QString::fromLatin1(APP_VERSION));
    MainWindow w;
    w.show();
    return a.exec();
}
