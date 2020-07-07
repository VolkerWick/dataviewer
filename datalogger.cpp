#include "datalogger.h"

#include <QStandardPaths>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

#include <QDebug>

const char DELIMITER = '\t';

DataLogger::DataLogger(QObject *parent)
    : QObject(parent)
{
}

QDir DataLogger::logFileDir()
{
    const QDir logFileDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

    if (!logFileDir.exists()) {
        logFileDir.mkpath(logFileDir.absolutePath());
    }

    return logFileDir;
}

bool DataLogger::open()
{
    QString path = logFileDir().absoluteFilePath(QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss").append(".log"));
    logFile = new QFile(path);
    if (logFile->open(QIODevice::WriteOnly|QIODevice::Text)) {
        logStream.setDevice(logFile);
        logStream.setRealNumberNotation(QTextStream::FixedNotation);

        return true;
    }

    qCritical() << "Unable to open LogFile" << path << logFile->errorString();
    return false;
}

void DataLogger::close()
{
    logFile->close();
    delete logFile;
    logFile = nullptr;
}

QString DataLogger::errorString() const
{
    if (logFile == nullptr) {
        return tr("Log File not initialized. Unable to obtain error.");
    }

    return logFile->errorString();
}

static QTextStream& operator<<(QTextStream& s, QList<QPointF>& dataPoints) {

    // first column: timestamp in unix format, no decimal places needed
    // use the first point's time stamp for this row
    s << qSetRealNumberPrecision(0) << dataPoints.first().rx() << DELIMITER;

    // for the actual data points show decimal places
    s << qSetRealNumberPrecision(2);

    for (auto point : dataPoints) {
        s << point.ry() << DELIMITER;
    }

    return s << Qt::endl;
}

void DataLogger::receiveDataRow(QList<QPointF> dataPoints)
{
    if (logStream.device()) {
        logStream << dataPoints;
    }
}
