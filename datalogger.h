#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QObject>
#include <QPointF>
#include <QTextStream>
#include <QDir>

class QFile;
class QTextStream;

class DataLogger : public QObject
{
    Q_OBJECT
public:
    explicit DataLogger(QObject *parent = nullptr);

    // OS-specific location for the log files (creates directory if it doesn't exist)
    static QDir logFileDir();

    // create a new log file (timestamp as name) and open it for subsequent logging
    bool open();

    // close the previously opened log file
    void close();

    QString errorString() const;

public slots:
    void receiveDataRow(QList<QPointF>);

private:
    QFile* logFile = nullptr;
    QTextStream logStream;
};

#endif // DATALOGGER_H
