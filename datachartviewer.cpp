#include "datachartviewer.h"

#include <QDateTime>
#include <QTimer>
#include <QRandomGenerator>

#include <QDebug>

using namespace QtCharts;

const int DELAY_MS = 50;

static QDateTime now() {
    return QDateTime::currentDateTime();
}

DataChartViewer::DataChartViewer()
    : series(new QLineSeries(this))
    , chart(new QChart)
    , xAxis(new QDateTimeAxis(this))
    , yAxis(new QValueAxis(this))
{
    series->setName("Test Data");

    chart = new QChart();
    chart->addSeries(series);

    xAxis = new QDateTimeAxis;
    xAxis->setTickCount(5);
    xAxis->setFormat("HH:mm:ss,z");
    xAxis->setTitleText("Time");
    xAxis->setRange(now(), now().addSecs(10));
    chart->addAxis(xAxis, Qt::AlignBottom);
    series->attachAxis(xAxis);

    yAxis = new QValueAxis;
    yAxis->setLabelFormat("%.3f");
    yAxis->setTitleText("Data");
    chart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(yAxis);

    setChart(chart);
    setRenderHint(QPainter::Antialiasing);
}

void DataChartViewer::receiveDataRow(QList<QPointF> dataPoints) {
    qDebug() << "received points" << dataPoints;

    if (dataPoints.length() > 1) {

        QPointF first = dataPoints.takeAt(1);

        series->append(first);
        if (series->count() > xAxis->min().msecsTo(xAxis->max())/DELAY_MS) {
            series->remove(0);
            xAxis->setMin(xAxis->min().addMSecs(DELAY_MS));
            xAxis->setMax(xAxis->max().addMSecs(DELAY_MS));
        }
    }

}
