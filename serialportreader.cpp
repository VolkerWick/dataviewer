#include "serialportreader.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDateTime>

#include <QDebug>

SerialPortReader::SerialPortReader(QObject *parent)
    : QObject(parent)
{
    QList<QSerialPortInfo> serialPortInfo = QSerialPortInfo::availablePorts();

    if (!serialPortInfo.isEmpty()) {
        port = new QSerialPort(QSerialPortInfo::availablePorts().first(), parent);

        connect(port, &QSerialPort::readyRead, this, [=]() {

            if (port->canReadLine()) {
                QDateTime stamp = QDateTime::currentDateTime();
                QByteArray line = port->readLine();

                QList<QPointF> dataPoints;
                QByteArrayList items = line.split(',');

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

        if (port->open(QIODevice::ReadOnly)) {
            qDebug() << "opened port" << port->portName() << "successfully.";
        }

    } else {
        qDebug() << "No Serial Port active.";
    }
}
