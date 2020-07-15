#include "datalogger.h"

#include <QStandardPaths>
#include <QDebug>

#include "serialportreader.h"

const int REALNUMBERPRECISION = 2;

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

static QTextStream& operator<<(QTextStream& s, const QList<float>& dataPoints) {

    for (auto point : dataPoints) {
        s << point << DELIMITER;
    }

    return s;
}

void DataLogger::receiveDataRow(QDateTime timeStamp, QList<float> dataSignal)
{
    if (logStream.device()) {
        logStream << timeStamp.toString("hh:mm:ss.zzz") << qSetRealNumberPrecision(REALNUMBERPRECISION) << DELIMITER << dataSignal

#if (QT_VERSION > QT_VERSION_CHECK(5, 12, 0))
    << Qt::endl;
#else
    << endl;
#endif
    }
}
