#include "datachartviewer.h"

#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>

#include <QDateTime>
#include <QTimer>
#include <QRandomGenerator>

using namespace QtCharts;

const int DELAY_MS = 50;

static QDateTime now() {
    return QDateTime::currentDateTime();
}


DataChartViewer::DataChartViewer()
{
    QLineSeries *series = new QLineSeries();
    series->setName("Test Data");

    QChart *chart = new QChart();
    chart->addSeries(series);

    QDateTimeAxis *xAxis = new QDateTimeAxis;
    xAxis->setTickCount(5);
    xAxis->setFormat("HH:mm:ss,z");
    xAxis->setTitleText("Time");
    xAxis->setRange(now(), now().addSecs(10));
    chart->addAxis(xAxis, Qt::AlignBottom);
    series->attachAxis(xAxis);

    QValueAxis *yAxis = new QValueAxis;
    yAxis->setLabelFormat("%.3f");
    yAxis->setTitleText("Data");
    chart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(yAxis);

    setChart(chart);
    setRenderHint(QPainter::Antialiasing);

    QTimer* timer = new QTimer;

    connect(timer, &QTimer::timeout, this, [=]() {
        series->append(now().toMSecsSinceEpoch(), QRandomGenerator::global()->generateDouble());
        if (series->count() > xAxis->min().msecsTo(xAxis->max())/DELAY_MS) {
            series->remove(0);
            xAxis->setMin(xAxis->min().addMSecs(DELAY_MS));
            xAxis->setMax(xAxis->max().addMSecs(DELAY_MS));
        }
    });

    timer->start(DELAY_MS);



}
