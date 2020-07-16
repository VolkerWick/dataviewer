#ifndef DATACHARTVIEWER_H
#define DATACHARTVIEWER_H

#include <QDateTime>
#include <QChartView>
using namespace QtCharts;

#include "layout.h"

class DataChartViewer : public QtCharts::QChartView
{
    Q_OBJECT
public:
    DataChartViewer(const ChartInfo& chartInfo);
    ~DataChartViewer();

public slots:
    void receiveDataRow(QDateTime timeStamp, QList<float> dataSignal);

signals:
    void sendDataRow(QDateTime timeStamp, QList<float> dataSignal);

private:
    QChart* chart;
};

#endif // DATACHARTVIEWER_H
