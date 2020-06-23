#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "datachartviewer.h"
#include "serialportreader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SerialPortReader* serialPortReader = new SerialPortReader(this);
    chartViewer = new DataChartViewer;

    connect(serialPortReader, &SerialPortReader::sendDataRow, chartViewer, &DataChartViewer::receiveDataRow);

    setCentralWidget(chartViewer);
}

MainWindow::~MainWindow()
{
    delete chartViewer;
    delete ui;
}

