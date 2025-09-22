#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "AppConstants.h"
#include "Popups/patientinfoentry.h"
#include "DAQ/writedata.h"

#include <QDebug>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <DAQ/readdata.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    generic_window_title = "Jeffryes-Solomon Low Cost Urodynamics Machine";
    setWindowTitle(generic_window_title);

    initConnections();
    calculateObjectSizes();
    setSizes();
    event_code = EventConstants::NO_EVENT;

    QDir curr_dir = QDir(".");
    curr_dir.cdUp();
    curr_dir.cd("data");
    data_writer.setFileDirectory(curr_dir.absolutePath());
    write_to_csv=false;
    csv_created=false;
    readSerialData = false;
    patient_entered = false;

    ui->graphDisplay->setBackgroundColour(30,30,30);
    ui->graphDisplay->setHorizontalAxesColour(QPen(QColor(255,255,255)));
    QVector<QPen> line_colours ={
        QPen(QColor(0,0,255)),
        QPen(QColor(255,0,0)),
        QPen(QColor(0,255,0)),
        QPen(QColor(255,165,0)),
        QPen(QColor(237,7,226)),
        QPen(QColor(255,255,0)),
        QPen(QColor(150,75,0))
    };
    ui->graphDisplay->setDataLineColours(line_colours);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnections(){
    to_zero_sensors = false;

    // 1 TEST SELECTION
    connect(ui->investigationBarT, &InvestigationTypeBar::sendTestType, this, &MainWindow::receiveTestType);
    connect(ui->investigationBarT, &InvestigationTypeBar::sendExitTestType, this, &MainWindow::receiveExitTestType);
    connect(ui->investigationBarT, &InvestigationTypeBar::sendTestSelected, ui->conBar, &ConnectionBar::receiveTestSelected);
    connect(ui->investigationBarT, &InvestigationTypeBar::sendTestType, ui->investigationBarC, &InvestigationBar::recieveTestingType);

    // 2 SERIAL CONNECTION
    conn = new QSerialPort();
    connect(conn, SIGNAL(readyRead()), this, SLOT(serialReceived()));
    connect(ui->conBar->getConnectionButton(), &QPushButton::clicked, this, &MainWindow::connectToSerialPort);
    connect(this, &MainWindow::sendConnectionOpen, ui->investigationBarT, &InvestigationTypeBar::receiveConnectionOpen);
    connect(ui->conBar, &ConnectionBar::sendSerialConnectionMade, ui->investigationBarC, &InvestigationBar::recieveSerialConnectionMade);

    // 3 PATIENT INFO
    connect(ui->investigationBarC, &InvestigationBar::sendOpenNewPatient, this, &MainWindow::receiveOpenNewPatient);
    connect(ui->investigationBarC, &InvestigationBar::sendOpenExistingPatient, this, &MainWindow::receiveOpenExisitngPatient);
    connect(ui->investigationBarC, &InvestigationBar::sendClosePatient, this, &MainWindow::receiveClosePatient);

    // 4 TEST SETUP
    connect(this, &MainWindow::sendEnablePumpButtons, ui->pump, &PumpControl::setPumpButtonsEnabled);
    connect(ui->investigationBarC, &InvestigationBar::sendZeroPressure, this, &MainWindow::recieveZeroPressure);

    // 5 TEST RECORDING
    connect(this, &MainWindow::sendEnterRecording, ui->conBar, &ConnectionBar::setEnterRecordingMode);
    connect(ui->investigationBarC, &InvestigationBar::sendStartRecording, this, &MainWindow::recieveStartRecordingCSV);
    connect(ui->investigationBarC, &InvestigationBar::sendStopRecording, this, &MainWindow::recieveStopRecordingCSV);

    // 6 EVENTS
    connect(ui->investigationBarC, &InvestigationBar::sendEvent, this, &MainWindow::recieveEventCode);
    connect(ui->pump, &PumpControl::sendPumpRate, this, &MainWindow::recievePumpValue);

    // 7 DATA LOGGING

}

// UI
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
    ui->pump->setContentsMargins(pixel_sizes["bar_padding_x"],pixel_sizes["bar_padding_y"],pixel_sizes["bar_padding_x"],pixel_sizes["bar_padding_y"]);

    ui->investigationBarT->setMinimumHeight(pixel_sizes["header_min"]);
    ui->investigationBarT->setMaximumHeight(pixel_sizes["header_max"]);
    ui->conBar->setMinimumHeight(pixel_sizes["header_min"]);
    ui->conBar->setMaximumHeight(pixel_sizes["header_max"]);
    ui->investigationBarC->setMinimumHeight(pixel_sizes["inforbar_min"]);
    ui->investigationBarC->setMaximumHeight(pixel_sizes["inforbar_max"]);
    ui->pump->setMinimumHeight(pixel_sizes["inforbar_min"]);
    ui->pump->setMaximumHeight(pixel_sizes["inforbar_max"]);
}

// 1 TEST SELECTION
void MainWindow::receiveTestType(QString test){
    this->test = test;
    setWindowTitle(test);

    data_reader.setTestingType(test);
    data_writer.setTestingType(test);
    ui->valueDisplay->setTestingType(test);
    ui->graphDisplay->setSampleWindowLength(SAMPLE_WINDOW_LENGTH);
    ui->graphDisplay->setTestingType(test);
    ui->pump->setTestingType(test);

    qInfo() << "Investigation setting loaded: " << test;
}

void MainWindow::receiveExitTestType(){
    setWindowTitle("JS Urodynamics Without Borders");

    ui->investigationBarC->setExitTestingType();
    ui->valueDisplay->setExitTestingType();
    ui->graphDisplay->setExitTestingType();
    ui->pump->setExitTestingType();

    qInfo() << "Investigation setting exited: " << test;
}

// 2 SERIAL CONNECTION
void MainWindow::setSerialConnection(){
    serialBuffer = "";
    portName = ui->conBar->getSelectedPort();
    conn->setPortName(portName);
    //conn->open(QSerialPort::ReadWrite);
    conn->open(QIODevice::ReadWrite);
    conn->setBaudRate(SerConnConstants::BAUD_RATE);
    conn->setDataBits(QSerialPort::Data8);
    conn->setParity(QSerialPort::NoParity);
    conn->setStopBits(QSerialPort::OneStop);
    conn->setFlowControl(QSerialPort::NoFlowControl);
    qInfo() << "Serial connection parameters set (" << portName << "-" << SerConnConstants::BAUD_RATE << ")";
}

void MainWindow::connectToSerialPort(){
    if (ui->conBar->isPortOpen()){
        qInfo() << "Open Serial connection";
        setSerialConnection();
        readSerialData = true;
        ui->investigationBarC->setPatientInfoEntered(patient_entered);

        emit sendConnectionOpen(true);
        if (test == "UDS Investigation"){
            emit sendSetUDSView();
        } else {
            emit sendSetTestView();
        }

        writeToSerialPort(SerConnConstants::START_DATA_TRANSMISSION);
        emit sendEnablePumpButtons(true);
    } else {
        qInfo() << "Close Serial connection";
        readSerialData = false;
        conn->close();
        //ui->investigationBarC->resetView();
        emit sendConnectionOpen(false);
        emit sendEnablePumpButtons(false);
    }
}

void MainWindow::closeSerialConnection(){
    writeToSerialPort(SerConnConstants::END_DATA_TRANSMISSION);
    ui->conBar->setDisconnected();
    conn->close();
    readSerialData = false;
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
            processIncomingData(bufferSplit.value(1));
        }
    }
}

void MainWindow::processIncomingData(QString data_string){

    // ####### edit event code

    std::map<QString, double> curr_dataset = data_reader.readCurrentDataset(data_string, event_code, to_zero_sensors);
    double infusion_flowrate = data_reader.getInfusionFlowrate();

    to_zero_sensors = false;

    ui->graphDisplay->updateEventLines();
    ui->valueDisplay->updateNumbers(curr_dataset);
    ui->graphDisplay->addDataset(curr_dataset);
    ui->pump->setPumpInfusionRate(infusion_flowrate);

    if (write_to_csv){data_writer.writeDataToCSV(curr_dataset);}

    serialBuffer = "";
    event_code = EventConstants::NO_EVENT;
}

void MainWindow::writeToSerialPort(int value){
    QByteArray q_b;
    if(conn->isOpen()){
        q_b.setNum(value);
        q_b.append('\n');
        conn->write(q_b);
        conn->waitForBytesWritten(-1);
    } else {
        QMessageBox::critical(this, tr("Write Error"), conn->errorString());
    }
}

// 3 patient controls

void MainWindow::receiveOpenNewPatient(){
    qInfo() << "Creating new patient popup";
    PatientInfoEntry patientInfoPopup;
    int dialogCode = patientInfoPopup.exec();

    if(dialogCode == QDialog::Accepted) {
        patient_entered = true;
        qInfo() << "Patient information entered successfully";
        std::map<QString, QString> patient_information = patientInfoPopup.getPatientInformation();
        setWindowTitle(patientInfoPopup.getWindowTitle());

        ui->investigationBarC->setPatientInfoEntered(readSerialData);
        data_writer.loadMetaData(patient_information);

    } else if (dialogCode == QDialog::Rejected) {
        patient_entered = false;
        ui->investigationBarC->setPatientInfoEntered(patient_entered);
        ui->investigationBarC->resetView();
        qInfo() << "Patient information entry failed";
    }
}

void MainWindow::receiveOpenExisitngPatient(){
    closeSerialConnection();
    // add code to clear graph and load patient
}

void MainWindow::receiveClosePatient(){
    /*ui->investigationBarC->resetView();
    if (test == "UDS Investigation"){
        emit sendSetUDSView();
    } else {
        emit sendSetTestView();
    }*/
    ui->investigationBarC->setPatientInfoEntered(false);
    setWindowTitle(generic_window_title);
}

// 4 Manage recording
void MainWindow::recieveStartRecordingCSV(){
    emit sendEnterRecording(true);
    data_writer.createCSVFile();
    write_to_csv=data_writer.checkCSVCreated();
}

void MainWindow::recieveStopRecordingCSV(){
    emit sendEnterRecording(false);
    data_writer.setEndCSVRecording();
    write_to_csv=data_writer.checkCSVCreated();
}

void MainWindow::recieveEventCode(int code){
    ui->graphDisplay->createEventLine(EventConstants::EVENTS.at(code));
    event_code = code;
}

void MainWindow::recieveZeroPressure(){data_reader.setZeroPressure();}

void MainWindow::recievePumpValue(int value){
    writeToSerialPort(value);
}
