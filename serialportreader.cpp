#include "serialportreader.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDateTime>

#include <QDebug>

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
                QDateTime timeStamp = QDateTime::currentDateTime();
                QByteArray line = port->readLine();

                QList<float> dataPoints;
                QByteArrayList items = line.split(DELIMITER);

                for (const QByteArray& item : items) {
                    float floatValue = item.toFloat();
                    dataPoints << floatValue;
                }

                // qDebug() << Q_FUNC_INFO << "send:" << timeStamp << dataPoints;
                // discard the first row of data in case it is incomplete
                if (firstEolEncountered) {
                    emit sendDataRow(timeStamp, dataPoints);
                }

                firstEolEncountered = true;
            }
        });

        connect(port, &QSerialPort::errorOccurred, this, [=](QSerialPort::SerialPortError e) {
            if (e != QSerialPort::NoError) {
                qCritical() << QDateTime::currentDateTime() << "Error " << e << "occurred: " << port->errorString();
            }
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
