#include "mainwindow.h"

#include <QApplication>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>
#include <QIcon>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication::setApplicationVersion("0.3.1");
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

    // read layout file
    QFile f(layoutFileName);

    if (!f.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QMessageBox(QMessageBox::Critical,
                    QApplication::applicationName(),
                    QObject::tr("Unable to open layoutfile \"%1\". Error: %2").arg(layoutFileName, f.errorString()))
                .exec();
        return -1;
    }

    // parse JSON
    QByteArray json = f.readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(json, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox(QMessageBox::Critical,
                    QApplication::applicationName(),
                    QObject::tr("Unable to parse layout \"%1\".\n%2.\nApproximate location: position %3")
                    .arg(layoutFileName, parseError.errorString()).arg(parseError.offset))
                .exec();

        return -1;
    }

    // pass layout object to main window which will set up the charts
    MainWindow w(doc.object());

    w.setWindowTitle(QApplication::applicationName());
    w.show();

    return a.exec();
}
