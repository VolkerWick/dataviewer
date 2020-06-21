#ifndef DATACHARTVIEWER_H
#define DATACHARTVIEWER_H

#include <QWidget>
#include <QChartView>


class DataChartViewer : public QtCharts::QChartView
{
    Q_OBJECT
public:
    DataChartViewer();
};

#endif // DATACHARTVIEWER_H
