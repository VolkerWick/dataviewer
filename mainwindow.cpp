#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "datachartviewer.h"
#include "serialportreader.h"

#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SerialPortReader* serialPortReader = new SerialPortReader(this);

    DataChartViewer* chart1 = new DataChartViewer({"Eins", "Zwei", "Drei"});
    chart1->setVisibleIndices({0,1,2});

    DataChartViewer* chart2 = new DataChartViewer({"Vier", "Fünf" });
    chart2->setVisibleIndices({3,4});

    DataChartViewer* chart3 = new DataChartViewer({"Sechs", "Sieben"});
    chart3->setVisibleIndices({5,6});

    DataChartViewer* chart4 = new DataChartViewer({"Acht", "Neun"});
    chart4->setVisibleIndices({7,8});

    connect(serialPortReader, &SerialPortReader::sendDataRow, chart1, &DataChartViewer::receiveDataRow);
    connect(serialPortReader, &SerialPortReader::sendDataRow, chart2, &DataChartViewer::receiveDataRow);
    connect(serialPortReader, &SerialPortReader::sendDataRow, chart3, &DataChartViewer::receiveDataRow);
    connect(serialPortReader, &SerialPortReader::sendDataRow, chart4, &DataChartViewer::receiveDataRow);

    QGridLayout* gridLayout = new QGridLayout;

    gridLayout->addWidget(chart1, 0, 0);
    gridLayout->addWidget(chart2, 0, 1);
    gridLayout->addWidget(chart3, 1, 0);
    gridLayout->addWidget(chart4, 1, 1);

    centralWidget()->setLayout(gridLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

