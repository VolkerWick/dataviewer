#include "datachartviewer.h"

#include <cmath>

#include <QDateTimeAxis>
#include <QLineSeries>
#include <QValueAxis>

#include <QDateTime>

using namespace QtCharts;

static QDateTime now() {
    return QDateTime::currentDateTime();
}

DataChartViewer::DataChartViewer(const ChartInfo& chartInfo)
    : chart(new QChart)
{
    chart->setTitle(chartInfo.getTitle());

    // one x axis for all series
    QDateTimeAxis* xAxis = new QDateTimeAxis(this);

    xAxis->setTickCount(5);
    xAxis->setFormat("HH:mm:ss,zzz");
    xAxis->setTitleText("Time");
    xAxis->setRange(now(), now().addSecs(10));
    chart->addAxis(xAxis, Qt::AlignBottom);

    int index = 0;
    for (auto info : chartInfo.getSignalInfo()) {

        QLineSeries* series = new QLineSeries(this);
        series->setName(info.getSignalName());
        series->setColor(info.getColor());

        chart->addSeries(series);
        series->attachAxis(xAxis);

        // if alignment is specified create new y Axis using alignment
        // otherwise attach existing axis to the series
        if (info.getAxisInfo().getAlignment() != ALIGN_INVALID) {
            QValueAxis* yAxis = new QValueAxis(this);

            yAxis->setLabelFormat("%.3f");
            yAxis->setTitleText(info.getAxisInfo().getTitle());

            chart->addAxis(yAxis, info.getAxisInfo().getAlignment());

            series->attachAxis(yAxis);
        } else {
            // no alignment given, use last axis for this series
            if (QValueAxis* lastAxis = dynamic_cast<QValueAxis*>(chart->axes(Qt::Vertical).last())) {
                series->attachAxis(lastAxis);
            }
        }
        ++index;
    }

    setChart(chart);
    setRenderHint(QPainter::Antialiasing);
}

DataChartViewer::~DataChartViewer()
{
    delete chart;
}

static bool less(const QPointF& left, const QPointF& right) {
    return left.y() < right.y();
}

static bool greater(const QPointF& left, const QPointF& right) {
    return left.y() > right.y();
}

// receiveDataRow consumes as many points as are needed to plot the signals for the owned graph
// and forwards the remaining list of points to the next DataChartViewer
void DataChartViewer::receiveDataRow(QDateTime timeStamp, QList<float> dataSignal) {

    // qDebug() << Q_FUNC_INFO <<  "receive:" << timeStamp << dataSignal;

    QDateTimeAxis* xAxis = dynamic_cast<QDateTimeAxis*>(chart->axes(Qt::Horizontal).first());

    for (int index = 0; index < chart->series().count(); index++) {

        QLineSeries* series = dynamic_cast<QLineSeries*>(chart->series().at(index));
        if (series != nullptr && !dataSignal.isEmpty()) {

            QPointF point(timeStamp.toMSecsSinceEpoch(), dataSignal.takeFirst());

            series->append(point);

            if (series->count() > 200) {
                QPointF leftmostPoint = series->at(0);
                series->remove(0);
                xAxis->setMin(QDateTime::fromMSecsSinceEpoch(leftmostPoint.rx()));
            }

            // slide x axis
            if (index == 0 && xAxis) {
                if (xAxis->max() < QDateTime::fromMSecsSinceEpoch(point.rx())) {
                    xAxis->setMax(QDateTime::fromMSecsSinceEpoch(point.rx()));
                }
            }

            if (series->attachedAxes().count() > 1) {
                // adjust y axis
                auto maxPos = std::max_element(series->pointsVector().cbegin(), series->pointsVector().cend(), less);
                auto minPos = std::max_element(series->pointsVector().cbegin(), series->pointsVector().cend(), greater);

                // first axis (index 0) is always xAxis
                QValueAxis* yAxis = dynamic_cast<QValueAxis*>(series->attachedAxes().at(1));
                yAxis->setMax(ceil(maxPos->y()));
                yAxis->setMin(floor(minPos->y()));
            }
        }
    }

    if (!dataSignal.isEmpty()) {
        emit sendDataRow(timeStamp, dataSignal);
    }
}

