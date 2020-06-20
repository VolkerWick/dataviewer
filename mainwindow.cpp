#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLineSeries>
#include <QChart>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QChartView>

#include <QDateTime>

#include <QDebug>
#include <QThread>
#include <QRandomGenerator>
#include <QTimer>

using namespace QtCharts;

const int DELAY_MS = 50;

static QDateTime now() {
    return QDateTime::currentDateTime();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    yAxis->setLabelFormat("%i");
    yAxis->setTitleText("Data");
    chart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(yAxis);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    setCentralWidget(chartView);
    resize(1400, 800);

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

MainWindow::~MainWindow()
{
    delete ui;
}

