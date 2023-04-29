#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    event_code = 1;

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
        QPen(QColor(230,230,255)),
        QPen(QColor(255,255,0)),
        QPen(QColor(150,75,0))
    };
    ui->valueDisplay->setVarColours(line_colours);
    ui->graphDisplay->setDataLineColours(line_colours);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnections(){
    // 1 select test type
    connect(ui->investigationBarT, &InvestigationTypeBar::sendTestName, this, &MainWindow::receiveTestType);
    connect(ui->investigationBarT, &InvestigationTypeBar::sendExitTest, this, &MainWindow::receiveExitTestType);
    connect(this, &MainWindow::sendConnectionOpen, ui->investigationBarT, &InvestigationTypeBar::receiveConnectionOpen);
    connect(ui->investigationBarT, &InvestigationTypeBar::sendTestSelected, ui->conBar, &ConnectionBar::receiveTestSelected);

    // 2 serial comms
    conn = new QSerialPort();
    connect(conn, SIGNAL(readyRead()), this, SLOT(serialReceived()));
    connect(ui->conBar->getConnectionButton(), &QPushButton::clicked, this, &MainWindow::connectToSerialPort);

    // 3 patient management
    connect(this, &MainWindow::sendSetUDSView, ui->investigationBarC, &InvestigationBar::setUDSView);
    connect(this, &MainWindow::sendSetTestView, ui->investigationBarC, &InvestigationBar::setTestView);
    connect(ui->investigationBarC, &InvestigationBar::sendOpenNewPatient, this, &MainWindow::receiveOpenNewPatient);
    connect(ui->investigationBarC, &InvestigationBar::sendOpenExistingPatient, this, &MainWindow::receiveOpenExisitngPatient);
    connect(ui->investigationBarC, &InvestigationBar::sendClosePatient, this, &MainWindow::receiveClosePatient);


    // 4 recording
    connect(ui->investigationBarC, &InvestigationBar::sendStartRecording, this, &MainWindow::recieveStartRecordingCSV);
    connect(ui->investigationBarC, &InvestigationBar::sendStopRecording, this, &MainWindow::recieveStopRecordingCSV);
    connect(this, &MainWindow::sendEnterRecording, ui->conBar, &ConnectionBar::setEnterRecordingMode);
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

    ui->investigationBarT->setMinimumHeight(pixel_sizes["header_min"]);
    ui->investigationBarT->setMaximumHeight(pixel_sizes["header_max"]);
    ui->conBar->setMinimumHeight(pixel_sizes["header_min"]);
    ui->conBar->setMaximumHeight(pixel_sizes["header_max"]);
    ui->investigationBarC->setMinimumHeight(pixel_sizes["inforbar_min"]);
    ui->investigationBarC->setMaximumHeight(pixel_sizes["inforbar_max"]);
}

// 1 Test type controls
void MainWindow::receiveTestType(QString test){
    this->test = test;
    setWindowTitle(test);

    data_reader.setTestingType(test);
    std::map<int, QString> var_names = data_reader.getChannelNames();
    QVector<QVector<double>> var_ranges = data_reader.getChannelRanges();

    data_writer.setVariableTitles(var_names);
    ui->valueDisplay->setDisplayChannels(var_names);

    ui->graphDisplay->setSampleWindowLength(200);
    ui->graphDisplay->setChannelNamesAndRanges(var_names, var_ranges);
    //ui->graphDisplay->setChannelNames(var_names, var_ranges, 200);
    ui->investigationBarC->setView(test);

    qInfo() << "Investigation setting loaded: " << test;
}

void MainWindow::receiveExitTestType(){
     ui->valueDisplay->displayReset();
     ui->investigationBarC->resetView();
     setWindowTitle("JS Urodynamics Without Borders");
     qInfo() << "Investigation setting exited: " << test;
}

// 2 Connection Settings
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
        ui->investigationBarC->setUDSReadyView(patient_entered);

        emit sendConnectionOpen(true);
        if (test == "UDS Investigation"){
            emit sendSetUDSView();
        } else {
            emit sendSetTestView();
        }
    } else {
        qInfo() << "Close Serial connection";
        readSerialData = false;
        conn->close();
        ui->investigationBarC->resetView();
        emit sendConnectionOpen(false);
    }
}

void MainWindow::closeSerialConnection(){
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
            processIncomingData(bufferSplit.value(1), event_code, false);
        }
    }

}

void MainWindow::processIncomingData(QString data_string, int event, bool zero_sensors){
    std::map<QString, double> curr_dataset = data_reader.readCurrentDataset(data_string, event_code, false);

    ui->valueDisplay->updateNumbers(curr_dataset);
    ui->graphDisplay->addDataset(curr_dataset);

    if (write_to_csv){data_writer.writeDataToCSV(curr_dataset);}

    serialBuffer = "";

    //qDebug() << event << zero_sensors;

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

        ui->investigationBarC->setUDSReadyView(readSerialData);
        data_writer.loadMetaData(patient_information);

    } else if (dialogCode == QDialog::Rejected) {
        patient_entered = false;
        ui->investigationBarC->setUDSReadyView(patient_entered);
        ui->investigationBarC->resetView();
        qInfo() << "Patient information entry failed";
    }
}

void MainWindow::receiveOpenExisitngPatient(){
    closeSerialConnection();

    // add code to clear graph and load patient
}

void MainWindow::receiveClosePatient(){
    ui->investigationBarC->resetView();
    if (test == "UDS Investigation"){
        emit sendSetUDSView();
    } else {
        emit sendSetTestView();
    }
    ui->investigationBarC->setUDSReadyView(false);
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