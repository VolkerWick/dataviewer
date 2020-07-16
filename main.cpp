#include "mainwindow.h"

#include <QApplication>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>
#include <QIcon>

#include <QDebug>

#include "layout.h"

int main(int argc, char *argv[])
{
    QApplication::setApplicationVersion("0.4.0");
    QApplication::setApplicationName("DataViewer");

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resource/dataviewer.ico"));

    // parse command line
    QString layoutFileName;
    for (auto arg : a.arguments()) {
        if (arg.startsWith("-layout=")) {
            layoutFileName = arg.split("=").at(1);
        }
    }

    if (layoutFileName.isEmpty()) {
        QMessageBox(QMessageBox::Critical,
                    QApplication::applicationName(),
                    QObject::tr("Please provide the path to a layout file.\nE.g. -layout=<layout-file-path>"))
                .exec();
        return -1;
    }

    try {
        Layout layout(layoutFileName);

        // pass layout object to main window which will set up the charts
        MainWindow w(layout);

        w.setWindowTitle(QApplication::applicationName());
        w.show();

        return a.exec();

    } catch (std::runtime_error* e) {
        QMessageBox(QMessageBox::Critical,
                    QApplication::applicationName(),
                    e->what())
                .exec();

        return -1;
    }
}
