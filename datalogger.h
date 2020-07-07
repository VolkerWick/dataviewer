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

    static QDir logFileDir();

    bool open();
    void close();

    QString errorString() const;

public slots:
    void receiveDataRow(QList<QPointF>);

private:
    QFile* logFile = nullptr;
    QTextStream logStream;
};

#endif // DATALOGGER_H
