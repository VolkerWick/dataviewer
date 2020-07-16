#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "datachartviewer.h"
#include "serialportreader.h"
#include "datalogger.h"

#include <QGridLayout>
#include <QPushButton>
#include <QToolButton>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>

#include <QDesktopServices>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(const Layout& layout, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // the serialPortReader will receive data from the USB port, timestamp it and send it row by row to the charts and the datalogger
    SerialPortReader* serialPortReader = new SerialPortReader(this);

    // datalogger receives signals and writes them to a log
    DataLogger* dataLogger = new DataLogger(this);

    connect(serialPortReader, &SerialPortReader::sendDataRow, dataLogger, &DataLogger::receiveDataRow);

    // set up charts to be displayed in a grid layout
    QGridLayout* gridLayout = new QGridLayout;

    // charts are connected in a signal chain; the first chart receives a row with all data points
    // it takes as many data points from the row and sends it to the next chart which takes its points from the row
    // sending the remainder on to the next chart etc.
    QObject* previousReceiver = serialPortReader;

    int x = 0, y = 0;

    // parse the layout.json chart by chart and extract the signal information
    // create data chart graph and configure it according to each chart's layout
    for (const ChartInfo& chartInfo : layout.getChartInfo()) {

        // construct chart objects and connect them in the signal chain and add them to the user interface
        DataChartViewer* chart = new DataChartViewer(chartInfo);

        // continue signal-chain
        connect(previousReceiver, SIGNAL(sendDataRow(QDateTime,QList<float>)),chart, SLOT(receiveDataRow(QDateTime,QList<float>)));
        previousReceiver = chart;

        gridLayout->addWidget(chart, x, y);

        ++x;
        if (x > 1) {
            x = 0; ++y;
        }
    }

    centralWidget()->setLayout(gridLayout);

    // set up status bar widgets...

    // ... combobox for USB ports

    QComboBox* portsComboBox = new QComboBox(this);
    portsComboBox->addItems(SerialPortReader::portNames());
    portsComboBox->setToolTip(tr("Available serial ports."));

    // ... refresh button for ports
    QToolButton* refresh = new QToolButton(this);
    refresh->setToolTip(tr("Refresh list of available serial ports."));
    refresh->setIcon(QIcon(":/resource/refresh.ico"));

    connect(refresh, &QPushButton::clicked, this, [=](){
        portsComboBox->clear();
        portsComboBox->addItems(SerialPortReader::portNames());
    });

    // ... error label (feedback from serialPortReader)
    QLabel* errorLabel = new QLabel(this);
    errorLabel->setToolTip(tr("Status messages from the serial port."));

    // ... button to browse the log file directory
    QPushButton* logDirLink = new QPushButton(DataLogger::logFileDir().absolutePath(), this);
    logDirLink->setToolTip(tr("Browse the log file directory."));

    // ... checkbox to turn logging on/off
    QCheckBox* startStopLog = new QCheckBox(tr("Logger active"), this);
    startStopLog->setCheckState(Qt::Checked);
    startStopLog->setToolTip(tr("Toggle logging."));

    connect(startStopLog, &QCheckBox::clicked, this, [=]() {
        if (startStopLog->isChecked()) {
            dataLogger->open();
        } else {
            dataLogger->close();
        }
    });

    connect(logDirLink, &QPushButton::clicked, this,[=]() {
        QDesktopServices::openUrl(DataLogger::logFileDir().absolutePath());
    });

    // ... button to connect/disconnect to/from serial port
    const QIcon connectIcon(":/resource/connect.ico");
    const QIcon disconnectIcon(":/resource/disconnect.ico");

    const QString connectToolTip = tr("Connect to selected serial port.");
    const QString disconnectToolTip = tr("Disconnect from serial port.");

    QToolButton* connectDisconnectButton = new QToolButton(this);
    connectDisconnectButton->setIcon(connectIcon);
    connectDisconnectButton->setToolTip(connectToolTip);

    connect(connectDisconnectButton, &QToolButton::clicked, this, [=]() {
        if (serialPortReader->isOpen()) {
            serialPortReader->close();
            dataLogger->close();
            connectDisconnectButton->setIcon(connectIcon);
            connectDisconnectButton->setToolTip(connectToolTip);
        } else {
            serialPortReader->open(portsComboBox->currentText());
            if (startStopLog->isChecked()) {
                dataLogger->open();
            }
            connectDisconnectButton->setIcon(disconnectIcon);
            connectDisconnectButton->setToolTip(disconnectToolTip);
        }

        errorLabel->setText(serialPortReader->errorString());
    });

    ui->statusbar->addWidget(portsComboBox);
    ui->statusbar->addWidget(refresh);
    ui->statusbar->addWidget(connectDisconnectButton);
    ui->statusbar->addWidget(errorLabel);
    ui->statusbar->addWidget(logDirLink);
    ui->statusbar->addWidget(startStopLog);
}

MainWindow::~MainWindow()
{
    delete ui;
}

