#include "datachartviewer.h"

#include <QDateTime>
#include <QTimer>
#include <QRandomGenerator>

#include <QDebug>

using namespace QtCharts;

static QDateTime now() {
    return QDateTime::currentDateTime();
}

DataChartViewer::DataChartViewer(const QStringList &signalNames)
    : series(new QLineSeries(this))
    , chart(new QChart)
    , xAxis(new QDateTimeAxis(this))
    , yAxis(new QValueAxis(this))
{
    for (auto name : signalNames) {
        series->setName(name);

        chart->addSeries(series);

        xAxis->setTickCount(5);
        xAxis->setFormat("HH:mm:ss,zzz");
        xAxis->setTitleText("Time");
        xAxis->setRange(now(), now().addSecs(10));
        chart->addAxis(xAxis, Qt::AlignBottom);
        series->attachAxis(xAxis);

        yAxis->setLabelFormat("%.3f");
        yAxis->setTitleText(name);
        chart->addAxis(yAxis, Qt::AlignLeft);
        series->attachAxis(yAxis);
    }

    setChart(chart);
    setRenderHint(QPainter::Antialiasing);
}

void DataChartViewer::receiveDataRow(QList<QPointF> dataPoints) {
    // qDebug() << "received points" << dataPoints;

    if (dataPoints.length() > 1) {

        QPointF newPoint = dataPoints.takeAt(0);

        // adjust yAxis range
        if (yAxis->max() < newPoint.ry()) {
            yAxis->setMax(newPoint.ry() * 1.1f);
        }

        if (yAxis->min() > newPoint.ry()) {
            yAxis->setMin(newPoint.ry() * 1.1f);
        }

        series->append(newPoint);

        if (xAxis->max().toMSecsSinceEpoch() < newPoint.rx()) {
            xAxis->setMax(QDateTime::fromMSecsSinceEpoch(newPoint.rx()));
        }

        // adjust xAxis range
        if (series->count() > 1000) {
            QPointF leftmostPoint = series->at(0);
            series->remove(0);
            xAxis->setMin(QDateTime::fromMSecsSinceEpoch(leftmostPoint.rx()));
        }
    }
}
