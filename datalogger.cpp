#include "datalogger.h"

#include <QStandardPaths>
#include <QDateTime>
#include <QDir>

#include <QFile>
#include <QTextStream>

#include <QDebug>

const char DELIMITER = '\t';

static QString logFilePath() {

    const QDir logFileDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

    if (!logFileDir.exists()) {
        logFileDir.mkpath(logFileDir.absolutePath());
    }

    return logFileDir.absoluteFilePath(QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss").append(".log"));
}

DataLogger::DataLogger(QObject *parent)
    : QObject(parent)
    , logFile(new QFile(logFilePath(), parent))
{
    if (!logFile->open(QIODevice::WriteOnly|QIODevice::Text)) {
        qCritical() << "Unable to open LogFile" << logFilePath() << logFile->errorString();
    } else {
        logStream.setDevice(logFile);
        logStream.setRealNumberNotation(QTextStream::FixedNotation);
    }
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