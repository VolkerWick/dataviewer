#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "datachartviewer.h"
#include "serialportreader.h"
#include "datalogger.h"

#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , icon(QIcon(":/resource/dataviewer.ico"))
{
    ui->setupUi(this);
    setWindowIcon(icon);

    SerialPortReader* serialPortReader = new SerialPortReader(this);
    DataLogger* dataLogger = new DataLogger(this);

    DataChartViewer* chart1 = new DataChartViewer({{"Amp-1", "red", SignalInfo::left }, {"Amp-2", "green"}, {"Amp-3", "blue"},
                                                   {"Volt-1", "darkRed", SignalInfo::right}, { "Volt-2", "darkGreen"}, { "Volt-3", "darkBlue"}});
    DataChartViewer* chart2 = new DataChartViewer({{"Sig-7", "cyan", SignalInfo::left}});
    DataChartViewer* chart3 = new DataChartViewer({{"Sig-8", "magenta", SignalInfo::left}});
    DataChartViewer* chart4 = new DataChartViewer({{"Sig-9","blue", SignalInfo::left}});

    // serialPortReader sends the received points to the first chart
    // each chart consumes as many points as needed and forwards 
    // remaining data points to the next chart
    connect(serialPortReader, &SerialPortReader::sendDataRow, chart1, &DataChartViewer::receiveDataRow);
    connect(chart1, &DataChartViewer::sendDataRow, chart2, &DataChartViewer::receiveDataRow);
    connect(chart2, &DataChartViewer::sendDataRow, chart3, &DataChartViewer::receiveDataRow);
    connect(chart3, &DataChartViewer::sendDataRow, chart4, &DataChartViewer::receiveDataRow);

    connect(serialPortReader, &SerialPortReader::sendDataRow, dataLogger, &DataLogger::receiveDataRow);

    QGridLayout* gridLayout = new QGridLayout;

    gridLayout->addWidget(chart1, 0, 0);
    gridLayout->addWidget(chart2, 0, 1);
    gridLayout->addWidget(chart3, 1, 0);
    gridLayout->addWidget(chart4, 1, 1);

    centralWidget()->setLayout(gridLayout);

    // set up status bar widgets
    QComboBox* portsComboBox = new QComboBox(this);
    portsComboBox->addItems(SerialPortReader::portNames());

    const QString strConnect = tr("Connect");
    const QString strDisconnect = tr("Disconnect");

    QLabel* errorLabel = new QLabel(this);

    QPushButton* logDirLink = new QPushButton(DataLogger::logFileDir().absolutePath(), this);
    logDirLink->setToolTip(tr("Open log dir"));

    connect(logDirLink, &QPushButton::clicked, this,[=]() {
        QDesktopServices::openUrl(DataLogger::logFileDir().absolutePath());
    });

    QPushButton* connectDisconnectButton = new QPushButton(strConnect, this);
    connect(connectDisconnectButton, &QPushButton::clicked, this, [=]() {
        if (serialPortReader->isOpen()) {
            serialPortReader->close();
            dataLogger->close();
            connectDisconnectButton->setText(strConnect);
        } else {
            serialPortReader->open(portsComboBox->currentText());
            dataLogger->open();
            connectDisconnectButton->setText(strDisconnect);

        }

        errorLabel->setText(serialPortReader->errorString());
    });

    ui->statusbar->addWidget(portsComboBox);
    ui->statusbar->addWidget(connectDisconnectButton);
    ui->statusbar->addWidget(errorLabel);
    ui->statusbar->addWidget(logDirLink);
}

MainWindow::~MainWindow()
{
    delete ui;
}

