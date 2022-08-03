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
    channel_zeros["Time"] = 0.0;
    channel_zeros["PSENS"] = 0.0;
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

// Test controls
void MainWindow::receiveTestType(QString test){
    setWindowTitle(test);
    data_reader.setTestingType(test);
    std::map<int, QString> channel_names = data_reader.getChannelNames();
    ui->valueDisplay->setDisplayChannels(channel_names);

    qInfo() << "Investigation setting loaded: " << test;
}

void MainWindow::receiveExitTestType(){
     ui->valueDisplay->displayReset();

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
    qDebug() << "Serial connection parameters set";
    data_reader.setTestingType("Pressure");
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
            std::map<QString, double> curr_dataset = data_reader.readCurrentDataset(bufferSplit.value(1), event_code, channel_zeros);

            qDebug() << "Time: " << curr_dataset["Time"] << " PSENS: " << curr_dataset["PSENS"];

            serialBuffer = "";
        }
    }

}
