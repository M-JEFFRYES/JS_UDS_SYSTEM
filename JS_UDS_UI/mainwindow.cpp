#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Popups/patientinfoentry.h"

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
    calculateObjectSizes();
    setSizes();
    event_code = 1;
    generic_window_title = "Jeffryes-Solomon Low Cost Urodynamics Machine";
    setWindowTitle(generic_window_title);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnections(){
    conn = new QSerialPort();
    connect(conn, SIGNAL(readyRead()), this, SLOT(serialReceived()));
    connect(ui->conBar->getConnectionButton(), &QPushButton::clicked, this, &MainWindow::connectToSerialPort);

    connect(ui->investigationBarT, &InvestigationTypeBar::sendTestName, this, &MainWindow::receiveTestType);
    connect(ui->investigationBarT, &InvestigationTypeBar::sendExitTest, this, &MainWindow::receiveExitTestType);
    connect(this, &MainWindow::sendConnectionOpen, ui->investigationBarT, &InvestigationTypeBar::receiveConnectionOpen);
    connect(ui->investigationBarT, &InvestigationTypeBar::sendTestSelected, ui->conBar, &ConnectionBar::receiveTestSelected);

    connect(ui->investigationBarC, &InvestigationBar::sendOpenNewPatient, this, &MainWindow::receiveOpenNewPatient);
    connect(ui->investigationBarC, &InvestigationBar::sendOpenExistingPatient, this, &MainWindow::receiveOpenExisitngPatient);
    connect(ui->investigationBarC, &InvestigationBar::sendClosePatient, this, &MainWindow::receiveClosPatient);
}

int MainWindow::calculatePixels(double length, double percentage){
    return (int) (length * percentage);
}

void MainWindow::calculateObjectSizes(){
    double min_percentage;
    double max_percentage;
    double win_height;

    win_height = height();
    pixel_sizes["window_height"] = win_height;
    pixel_sizes["window_width"] = width();

    pixel_sizes["bar_padding_x"] = 2;
    pixel_sizes["bar_padding_y"] = 2;

    // TOP BAR
    min_percentage = 0.10;
    max_percentage = 0.15;
    pixel_sizes["header_min"] = calculatePixels(win_height, min_percentage);
    pixel_sizes["header_max"] = calculatePixels(win_height, max_percentage);

    // INVESTIGATION BAR
    min_percentage = 0.15;
    max_percentage = 0.20;
    pixel_sizes["inforbar_min"] = calculatePixels(win_height, min_percentage);
    pixel_sizes["inforbar_max"] = calculatePixels(win_height, max_percentage);

    // PLOTS


    //FOOTER
    min_percentage = 0.1;
    max_percentage = 0.15;
    pixel_sizes["footer_min"] = calculatePixels(win_height, min_percentage);
    pixel_sizes["footer_max"] = calculatePixels(win_height, max_percentage);
}

void MainWindow::setSizes(){
    ui->investigationBarT->setContentsMargins(pixel_sizes["bar_padding_x"],pixel_sizes["bar_padding_y"],pixel_sizes["bar_padding_x"],pixel_sizes["bar_padding_y"]);
    ui->conBar->setContentsMargins(pixel_sizes["bar_padding_x"],pixel_sizes["bar_padding_y"],pixel_sizes["bar_padding_x"],pixel_sizes["bar_padding_y"]);
    ui->investigationBarC->setContentsMargins(pixel_sizes["bar_padding_x"],pixel_sizes["bar_padding_y"],pixel_sizes["bar_padding_x"],pixel_sizes["bar_padding_y"]);

    ui->investigationBarT->setMinimumHeight(pixel_sizes["header_min"]);
    ui->investigationBarT->setMaximumHeight(pixel_sizes["header_max"]);
    ui->conBar->setMinimumHeight(pixel_sizes["header_min"]);
    ui->conBar->setMaximumHeight(pixel_sizes["header_max"]);
    ui->investigationBarC->setMinimumHeight(pixel_sizes["inforbar_min"]);
    ui->investigationBarC->setMaximumHeight(pixel_sizes["inforbar_max"]);
}



// Test controls
void MainWindow::receiveTestType(QString test){
    this->test = test;
    setWindowTitle(test);
    data_reader.setTestingType(test);
    std::map<int, QString> var_names = data_reader.getChannelNames();
    QVector<QVector<double>> var_ranges = data_reader.getChannelRanges();
    ui->valueDisplay->setDisplayChannels(var_names);
    ui->graphDisplay->setChannelNames(var_names, var_ranges, 200);
    ui->investigationBarC->setView(test);

    qInfo() << "Investigation setting loaded: " << test;
}

void MainWindow::receiveExitTestType(){
     ui->valueDisplay->displayReset();
     ui->investigationBarC->resetView();
     setWindowTitle("JS Urodynamics Without Borders");
     qInfo() << "Investigation setting exited: " << test;
}

// patient controls

void MainWindow::receiveOpenNewPatient(){
    qInfo() << "Creating new patient popup";
    PatientInfoEntry patientInfoPopup;
    int dialogCode = patientInfoPopup.exec();

    if(dialogCode == QDialog::Accepted) {
        qInfo() << "Patient information entered successfully";
        std::map<QString, QString> patient_information = patientInfoPopup.getPatientInformation();
        setWindowTitle(patientInfoPopup.getWindowTitle());
        ui->investigationBarC->setUDSReadyView();

        // Manage Pat info

    } else if (dialogCode == QDialog::Rejected) {
        ui->investigationBarC->resetView();
        qInfo() << "Patient information entry failed";
    }
}

void MainWindow::receiveOpenExisitngPatient(){

}

void MainWindow::receiveClosPatient(){
    ui->investigationBarC->resetView();
    setWindowTitle(generic_window_title);
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

        emit sendConnectionOpen(true);

    } else {
        qInfo() << "Close Serial connection";
        readSerialData = false;
        conn->close();
        emit sendConnectionOpen(false);
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
            processIncomingData(bufferSplit.value(1), event_code, false);
        }
    }

}

void MainWindow::processIncomingData(QString data_string, int event, bool zero_sensors){
    std::map<QString, double> curr_dataset = data_reader.readCurrentDataset(data_string, event_code, false);
    qDebug() << curr_dataset["PBLAD"];
    ui->valueDisplay->updateNumbers(curr_dataset);
    ui->graphDisplay->addDataset(curr_dataset);

    serialBuffer = "";

    qDebug() << event << zero_sensors;

}
