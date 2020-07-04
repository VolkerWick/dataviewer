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

public slots:
    void receiveDataRow(QList<QPointF>);

private:
    QFile* logFile;
    QTextStream logStream;
};

#endif // DATALOGGER_H
