#ifndef DATACHARTVIEWER_H
#define DATACHARTVIEWER_H

#include <QDateTime>
#include <QChartView>
using namespace QtCharts;

#include "signalinfo.h"

class DataChartViewer : public QtCharts::QChartView
{
    Q_OBJECT
public:

    // set up the chart viewer as configured by the parameter
    DataChartViewer(const QList<SignalInfo>& signalInfo);
    ~DataChartViewer();

public slots:
    void receiveDataRow(QDateTime timeStamp, QList<float> dataSignal);

signals:
    void sendDataRow(QDateTime timeStamp, QList<float> dataSignal);

private:
    QChart* chart;
};

#endif // DATACHARTVIEWER_H
