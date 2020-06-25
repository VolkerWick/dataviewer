#ifndef DATACHARTVIEWER_H
#define DATACHARTVIEWER_H

#include <QWidget>
#include <QChartView>
#include <QPointF>

#include <QLineSeries>
#include <QChart>
#include <QDateTimeAxis>
#include <QValueAxis>


using namespace QtCharts;

class DataChartViewer : public QtCharts::QChartView
{
    Q_OBJECT
public:
    DataChartViewer(const QStringList& signalNames);

public slots:
    void receiveDataRow(QList<QPointF>);

private:
    QLineSeries* series;
    QChart* chart;
    QDateTimeAxis* xAxis;
    QValueAxis* yAxis;

};

#endif // DATACHARTVIEWER_H
