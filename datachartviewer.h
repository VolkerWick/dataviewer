#ifndef DATACHARTVIEWER_H
#define DATACHARTVIEWER_H

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
    void receiveDataRow(QList<QPointF>);

signals:
    void sendDataRow(QList<QPointF>);

private:
    QChart* chart;
};

#endif // DATACHARTVIEWER_H
