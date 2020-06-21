#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "datachartviewer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(new DataChartViewer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

