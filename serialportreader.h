#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QObject>
#include <QDateTime>

// character that separates subsequent data values
#define DELIMITER '\t'

class QSerialPort;

class SerialPortReader : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortReader(QObject *parent = nullptr);

    static QStringList portNames();

    bool open(const QString& portName);
    void close();

    bool isOpen() const;

    QString errorString() const;

signals:
    // send time-stamped row of data signals
    void sendDataRow(QDateTime timeStamp, QList<float> dataSignal);

private:
    QSerialPort* port;
};

#endif // SERIALPORTREADER_H
