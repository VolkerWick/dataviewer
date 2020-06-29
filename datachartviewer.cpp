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
    : chart(new QChart)
{
    // one x axis for all series
    QDateTimeAxis* xAxis = new QDateTimeAxis(this);

    xAxis->setTickCount(5);
    xAxis->setFormat("HH:mm:ss,zzz");
    xAxis->setTitleText("Time");
    xAxis->setRange(now(), now().addSecs(10));
    chart->addAxis(xAxis, Qt::AlignBottom);

    int index = 0;
    for (auto name : signalNames) {
        QLineSeries* series = new QLineSeries(this);
        series->setName(name);

        chart->addSeries(series);
        series->attachAxis(xAxis);

        QValueAxis* yAxis = new QValueAxis(this);

        yAxis->setLabelFormat("%.3f");
        yAxis->setTitleText(name);
        if (index % 2 == 0) {
            chart->addAxis(yAxis, Qt::AlignLeft);
        } else {
            chart->addAxis(yAxis, Qt::AlignRight);
        }
        series->attachAxis(yAxis);
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

void DataChartViewer::receiveDataRow(QList<QPointF> dataPoints) {
    // qDebug() << "received points" << dataPoints;

    QDateTimeAxis* xAxis = dynamic_cast<QDateTimeAxis*>(chart->axes(Qt::Horizontal).first());

    int index = 0;

    for (auto point : dataPoints) {
        if (index < chart->series().count()) {
            QLineSeries* series = dynamic_cast<QLineSeries*>(chart->series().at(index));
            if (series != nullptr) {

                series->append(point);

                if (series->count() > 800) {
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

                // adjust y axis
                auto maxPos = std::max_element(series->pointsVector().cbegin(), series->pointsVector().cend(), less);
                auto minPos = std::max_element(series->pointsVector().cbegin(), series->pointsVector().cend(), greater);

                QValueAxis* yAxis = dynamic_cast<QValueAxis*>(series->attachedAxes().at(1));
                yAxis->setMax(ceil(maxPos->y()));
                yAxis->setMin(floor(minPos->y()));

            }
        }

        ++index;
    }
}
