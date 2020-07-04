#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setApplicationVersion("0.1.0");
    QApplication::setApplicationName("DataViewer");

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QApplication::applicationName());
    w.show();
    return a.exec();
}
