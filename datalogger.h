#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QObject>
#include <QPointF>
#include <QTextStream>

class QFile;
class QTextStream;

class DataLogger : public QObject
{
    Q_OBJECT
public:
    explicit DataLogger(QObject *parent = nullptr);

    // full path of log file
    QString logFileAbsolutePath() const;

    // log file's containing directory
    QString logFilePath() const;

    // log file's name
    QString logFileName() const;

public slots:
    void receiveDataRow(QList<QPointF>);

private:
    QString _logFilePath;
    QFile* logFile;
    QTextStream logStream;
};

#endif // DATALOGGER_H
