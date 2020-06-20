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

const int DELAY_MS = 100;

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

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("hh:mm:ss,z");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Data");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    setCentralWidget(chartView);
    resize(820, 600);

    QTimer* timer = new QTimer;

    axisX->setRange(now(), now().addSecs(10));

    connect(timer, &QTimer::timeout, this, [=]() {
        series->append(now().toMSecsSinceEpoch(), QRandomGenerator::global()->generateDouble());
    });

    timer->start(DELAY_MS);
}

MainWindow::~MainWindow()
{
    delete ui;
}

