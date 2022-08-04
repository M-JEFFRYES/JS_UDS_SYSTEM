#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <DAQ/readdata.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initConnections();
    setSizes();
    event_code = 1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnections(){
    conn = new QSerialPort();
    connect(conn, SIGNAL(readyRead()), this, SLOT(serialReceived()));
    connect(ui->conBar->getConnectionButton(), &QPushButton::clicked, this, &MainWindow::connectToSerialPort);
    connect(ui->InvestigationBar, &InvestigationTypeBar::sendTestName, this, &MainWindow::receiveTestType);
    connect(ui->InvestigationBar, &InvestigationTypeBar::sendExitTest, this, &MainWindow::receiveExitTestType);

}

void MainWindow::setSizes(){
    std::map<QString, int> sizes;
    sizes["header_min"] = 25;
    sizes["header_max"] = 200;
    sizes["inforbar_min"] = 25;
    sizes["inforbar_max"] = 200;

    sizes["footer_min"] = 25;
    sizes["footer_max"] = 200;

    ui->InvestigationBar->setMinimumHeight(sizes["header_min"]);
    ui->InvestigationBar->setMaximumHeight(sizes["header_max"]);
    ui->conBar->setMinimumHeight(sizes["header_min"]);
    ui->conBar->setMaximumHeight(sizes["header_max"]);

    ui->investigationControlBar->setMinimumHeight(sizes["inforbar_min"]);
    ui->investigationControlBar->setMaximumHeight(sizes["inforbar_max"]);
    ui->patientBar->setMinimumHeight(sizes["inforbar_min"]);
    ui->patientBar->setMaximumHeight(sizes["inforbar_max"]);
}


// Test controls
void MainWindow::receiveTestType(QString test){
    this->test = test;
    setWindowTitle(test);
    data_reader.setTestingType(test);
    std::map<int, QString> var_names = data_reader.getChannelNames();
    QVector<QVector<double>> var_ranges = data_reader.getChannelRanges();
    ui->valueDisplay->setDisplayChannels(var_names);
    ui->graphDisplay->setChannelNames(var_names, var_ranges);

    qInfo() << "Investigation setting loaded: " << test;
}

void MainWindow::receiveExitTestType(){
     ui->valueDisplay->displayReset();
     setWindowTitle("JS Urodynamics Without Borders");
     qInfo() << "Investigation setting exited: " << test;
}


// Serial Communication
void MainWindow::setSerialConnection(){
    serialBuffer = "";
    baudRate = 9600;
    portName = ui->conBar->getSelectedPort();
    conn->setPortName(portName);
    conn->open(QIODevice::ReadWrite);
    conn->setBaudRate(baudRate);
    conn->setDataBits(QSerialPort::Data8);
    conn->setParity(QSerialPort::NoParity);
    conn->setStopBits(QSerialPort::OneStop);
    conn->setFlowControl(QSerialPort::NoFlowControl);
    qInfo() << "Serial connection parameters set (" << portName << "-" << baudRate << ")";
}

void MainWindow::connectToSerialPort(){
    if (ui->conBar->isPortOpen()){
        qInfo() << "Open Serial connection";
        setSerialConnection();
        readSerialData = true;

    } else {
        qInfo() << "Close Serial connection";
        readSerialData = false;
        conn->close();
    }
}

void MainWindow::serialReceived(){

    if (readSerialData) {
        // read serial data and append to buffer
        serialData = conn->readAll();
        serialBuffer += QString::fromStdString(serialData.toStdString());

        // split the buffer by exit chars
        QStringList bufferSplit = serialBuffer.split("\r\n");

        if (bufferSplit.length() < 3){
            // wait until buffer has three chunks
            serialData = conn->readAll();
            serialBuffer += QString::fromStdString(serialData.toStdString());

        } else {

            std::map<QString, double> curr_dataset = data_reader.readCurrentDataset(bufferSplit.value(1), event_code, false);

            ui->valueDisplay->updateNumbers(curr_dataset);

            serialBuffer = "";
        }
    }

}
