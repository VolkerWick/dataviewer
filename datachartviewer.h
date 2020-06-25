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
    ~DataChartViewer();

public slots:
    void receiveDataRow(QList<QPointF>);

private:
    QChart* chart;

};

#endif // DATACHARTVIEWER_H
