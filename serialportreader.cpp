#include "serialportreader.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDateTime>

#include <QDebug>

const char DELIMITER = ';';

SerialPortReader::SerialPortReader(QObject *parent)
    : QObject(parent)
    , port(nullptr)
{
}

QStringList SerialPortReader::portNames()
{
    QStringList result;

    for (auto info : QSerialPortInfo::availablePorts()) {
        result << info.portName();
    }

    return result;
}

bool SerialPortReader::open(const QString &portName)
{
    if ((port = new QSerialPort(portName, this))) {

        connect(port, &QSerialPort::readyRead, this, [=]() {

            if (port->canReadLine()) {
                QDateTime stamp = QDateTime::currentDateTime();
                QByteArray line = port->readLine();

                QList<QPointF> dataPoints;
                QByteArrayList items = line.split(DELIMITER);

                for (const QByteArray& item : items) {
                    float floatValue = item.toFloat();
                    dataPoints << QPointF(stamp.toMSecsSinceEpoch(), floatValue);
                }

                emit sendDataRow(dataPoints);
            }
        });

        connect(port, &QSerialPort::errorOccurred, this, [=](QSerialPort::SerialPortError e) {
            qDebug() << QDateTime::currentDateTime() << "Error " << e << "occurred: " << port->errorString();
        });

        return port->open(QIODevice::ReadOnly);

    } else {
        return false;
    }
}

void SerialPortReader::close()
{
    port->close();
    port->disconnect();
}

bool SerialPortReader::isOpen() const
{
    if (port == nullptr) {
        return false;
    }

    return port->isOpen();
}

QString SerialPortReader::errorString() const
{
    if (port == nullptr) {
        return tr("Serial Port Reader not initialized. Unable to obtain error");
    }

    return port->errorString();
}
