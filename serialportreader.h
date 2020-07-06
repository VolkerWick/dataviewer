#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QObject>
#include <QPointF>

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
    void sendDataRow(QList<QPointF>);

private:
    QSerialPort* port;
};

#endif // SERIALPORTREADER_H
