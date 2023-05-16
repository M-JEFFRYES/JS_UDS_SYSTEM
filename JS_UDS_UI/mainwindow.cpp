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
    event_code = 0;

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
    connect(ui->pump, &PumpControl::sendPumpRate, this, &MainWindow::recievePumpValue);


    // 4 recording
    connect(ui->investigationBarC, &InvestigationBar::sendStartRecording, this, &MainWindow::recieveStartRecordingCSV);
    connect(ui->investigationBarC, &InvestigationBar::sendStopRecording, this, &MainWindow::recieveStopRecordingCSV);
    connect(this, &MainWindow::sendEnterRecording, ui->conBar, &ConnectionBar::setEnterRecordingMode);
    connect(ui->investigationBarC, &InvestigationBar::sendEvent, this, &MainWindow::recieveEventCode);
    connect(ui->investigationBarC, &InvestigationBar::sendZeroPressure, this, &MainWindow::recieveZeroPressure);
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

// 1 Test type controls
void MainWindow::receiveTestType(QString test){
    this->test = test;
    setWindowTitle(test);

    data_reader.setTestingType(test);
    std::map<int, QString> var_names = data_reader.getChannelNames();
    std::map<QString, int> plot_numbers = data_reader.getChannelPlotNumbers();
    QVector<QVector<double>> var_ranges = data_reader.getChannelRanges();
    QVector<QString> event_codes = data_reader.getEventCodes();
    QVector<QString> event_labels = data_reader.getEventLabels();

    data_writer.setVariableTitles(var_names);
    ui->valueDisplay->setDisplayChannels(var_names);
    ui->graphDisplay->setSampleWindowLength(200);
    ui->graphDisplay->setChannelNamesAndRanges(var_names, var_ranges, event_labels, plot_numbers);
    ui->investigationBarC->setView(event_codes);

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
    //conn->open(QSerialPort::ReadWrite);
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

        writeToSerialPort(333);
    } else {
        qInfo() << "Close Serial connection";
        readSerialData = false;
        conn->close();
        ui->investigationBarC->resetView();
        emit sendConnectionOpen(false);
    }
}

void MainWindow::closeSerialConnection(){
    writeToSerialPort(444);
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
            processIncomingData(bufferSplit.value(1), false);
        }
    }

}

void MainWindow::processIncomingData(QString data_string, bool zero_sensors){
    std::map<QString, double> curr_dataset = data_reader.readCurrentDataset(data_string, event_code, to_zero_sensors);
    to_zero_sensors = false;

    ui->graphDisplay->updateEventLines();
    ui->valueDisplay->updateNumbers(curr_dataset);
    ui->graphDisplay->addDataset(curr_dataset);

    if (write_to_csv){data_writer.writeDataToCSV(curr_dataset);}

    serialBuffer = "";
    event_code = 0;

    //qDebug() << event << zero_sensors;

}

void MainWindow::writeToSerialPort(int value){
    QByteArray q_b;
    q_b.setNum(value);
    q_b.append('\n');
    qInfo() << q_b;
    //QString val = QString::number(value);
    if(conn->isOpen()){
        //conn->write(val.toStdString().c_str());
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
     /*
    double data[200][6] = {{30.17,30.79,-0.6199999999999974,0.0,0.0,0.0},{29.21,30.53,-1.3200000000000003,3.0,0.0,0.0},{29.96,30.2,-0.23999999999999844,6.0,0.0,0.0},{29.25,29.66,-0.41000000000000014,9.0,0.0,0.0},{30.71,30.45,0.26000000000000156,12.0,0.0,0.0},{29.04,30.74,-1.6999999999999993,15.0,0.0,0.0},{30.34,30.43,-0.08999999999999986,18.0,0.0,0.0},{30.0,29.65,0.3500000000000014,21.0,0.0,0.0},{29.38,30.44,-1.0600000000000023,24.0,0.0,0.0},{30.85,30.05,0.8000000000000007,27.0,0.0,0.0},{30.36,30.12,0.23999999999999844,30.0,0.0,0.0},{29.42,29.34,0.08000000000000185,33.0,0.0,0.0},{30.05,30.16,-0.10999999999999943,36.0,0.0,0.0},{30.53,30.27,0.26000000000000156,39.0,0.0,0.0},{29.66,30.92,-1.2600000000000016,42.0,0.0,0.0},{29.66,29.82,-0.16000000000000014,45.0,0.0,0.0},{30.08,30.25,-0.1700000000000017,48.0,0.0,0.0},{30.46,29.11,1.3500000000000014,51.0,0.0,0.0},{29.31,30.33,-1.0199999999999996,54.0,0.0,0.0},{30.98,29.08,1.9000000000000021,57.0,0.0,0.0},{30.05,29.63,0.4200000000000017,60.0,0.0,0.0},{30.88,30.24,0.6400000000000006,63.0,0.0,0.0},{29.75,29.34,0.41000000000000014,66.0,0.0,0.0},{30.33,29.81,0.5199999999999996,69.0,0.0,0.0},{30.53,29.4,1.1300000000000026,72.0,0.0,0.0},{30.67,30.37,0.3000000000000007,75.0,0.0,0.0},{29.09,30.43,-1.3399999999999999,78.0,0.0,0.0},{29.42,29.21,0.21000000000000085,81.0,0.0,0.0},{30.74,29.23,1.509999999999998,84.0,0.0,0.0},{29.34,30.58,-1.2399999999999984,87.0,0.0,0.0},{30.91,30.28,0.629999999999999,90.0,0.0,0.0},{29.06,29.68,-0.620000000000001,93.0,0.0,0.0},{29.11,29.3,-0.19000000000000128,96.0,0.0,0.0},{29.04,29.9,-0.8599999999999994,99.0,0.0,0.0},{29.59,30.28,-0.6900000000000013,102.0,0.0,0.0},{30.43,30.48,-0.05000000000000071,105.0,0.0,0.0},{29.92,30.57,-0.6499999999999986,108.0,0.0,0.0},{30.26,29.48,0.7800000000000011,111.0,0.0,0.0},{30.94,30.39,0.5500000000000007,114.0,0.0,0.0},{30.88,29.9,0.9800000000000004,117.0,0.0,0.0},{29.47,30.4,-0.9299999999999997,120.0,0.0,0.0},{30.27,30.44,-0.1700000000000017,123.0,0.0,0.0},{30.01,29.93,0.08000000000000185,126.0,0.0,0.0},{30.91,29.11,1.8000000000000007,129.0,0.0,0.0},{30.74,29.54,1.1999999999999993,132.0,0.0,0.0},{30.92,30.29,0.6300000000000026,135.0,0.0,0.0},{29.46,30.58,-1.1199999999999974,138.0,0.0,0.0},{29.44,29.22,0.22000000000000242,141.0,0.0,0.0},{29.99,29.42,0.5699999999999967,144.0,0.0,0.0},{30.09,29.0,1.0899999999999999,147.0,0.0,0.0},{30.49,29.54,0.9499999999999993,150.0,0.0,0.0},{29.27,29.58,-0.3099999999999987,153.0,0.0,0.0},{30.37,30.35,0.019999999999999574,156.0,0.0,0.0},{30.82,29.85,0.9699999999999989,159.0,0.0,0.0},{29.09,29.35,-0.26000000000000156,162.0,0.0,0.0},{29.31,29.05,0.259999999999998,165.0,0.0,0.0},{29.26,29.52,-0.259999999999998,168.0,0.0,0.0},{30.91,29.35,1.5599999999999987,171.0,0.0,0.0},{29.15,30.84,-1.6900000000000013,174.0,0.0,0.0},{29.53,30.8,-1.2699999999999996,177.0,0.0,0.0},{30.6,29.9,0.7000000000000028,180.0,0.0,0.0},{29.26,30.51,-1.25,183.0,0.0,0.0},{30.13,30.1,0.029999999999997584,186.0,0.0,0.0},{29.0,30.34,-1.3399999999999999,189.0,0.0,0.0},{30.16,30.26,-0.10000000000000142,192.0,0.0,0.0},{29.43,29.68,-0.25,195.0,0.0,0.0},{30.38,30.0,0.379999999999999,198.0,0.0,0.0},{29.68,30.36,-0.6799999999999997,201.0,0.0,0.0},{29.08,29.35,-0.2700000000000031,204.0,0.0,0.0},{29.68,30.72,-1.0399999999999991,207.0,0.0,0.0},{29.94,30.04,-0.09999999999999787,210.0,0.0,0.0},{29.08,30.53,-1.4500000000000028,213.0,0.0,0.0},{29.64,30.06,-0.41999999999999815,216.0,0.0,0.0},{29.31,30.27,-0.9600000000000009,219.0,0.0,0.0},{29.75,30.4,-0.6499999999999986,222.0,0.0,0.0},{29.38,29.18,0.1999999999999993,225.0,0.0,0.0},{29.94,30.62,-0.6799999999999997,228.0,0.0,0.0},{30.58,30.68,-0.10000000000000142,231.0,0.0,0.0},{30.98,30.59,0.39000000000000057,234.0,0.0,0.0},{30.65,30.83,-0.17999999999999972,237.0,0.0,0.0},{29.03,29.45,-0.41999999999999815,240.0,0.0,0.0},{29.26,30.55,-1.2899999999999991,243.0,0.0,0.0},{30.71,30.67,0.03999999999999915,246.0,0.0,0.0},{30.6,30.29,0.3100000000000023,249.0,0.0,0.0},{30.39,29.26,1.129999999999999,252.0,0.0,0.0},{30.38,30.16,0.21999999999999886,255.0,0.0,0.0},{29.4,30.28,-0.8800000000000026,258.0,0.0,0.0},{29.67,29.34,0.33000000000000185,261.0,0.0,0.0},{30.11,29.04,1.0700000000000003,264.0,0.0,0.0},{29.6,30.92,-1.3200000000000003,267.0,0.0,0.0},{30.92,29.33,1.5900000000000034,270.0,0.0,0.0},{30.48,30.17,0.3099999999999987,273.0,0.0,0.0},{30.95,30.61,0.33999999999999986,276.0,0.0,0.0},{30.36,30.4,-0.03999999999999915,279.0,0.0,0.0},{29.57,30.47,-0.8999999999999986,282.0,0.0,0.0},{30.32,30.51,-0.19000000000000128,285.0,0.0,0.0},{29.01,30.37,-1.3599999999999994,288.0,0.0,0.0},{29.83,30.54,-0.7100000000000009,291.0,0.0,0.0},{29.45,29.91,-0.46000000000000085,294.0,0.0,0.0},{29.49,30.08,-0.5899999999999999,297.0,0.0,0.0},{42.48853622595717,29.78,12.708536225957168,300.0,1.0,0.0},{50.69711402007164,29.83,20.86711402007164,303.0,3.0,0.0},{42.88853622595717,31.0,11.888536225957168,306.0,5.0,0.0},{32.999548325659404,30.15,2.8495483256594056,309.0,5.0,0.0},{29.73,30.35,-0.620000000000001,312.0,5.0,0.0},{29.57,30.07,-0.5,315.0,5.0,0.0},{30.79,29.71,1.0799999999999983,318.0,5.0,0.0},{29.35,30.13,-0.7799999999999976,321.0,5.0,0.0},{30.01,30.77,-0.759999999999998,324.0,5.0,0.0},{47.617804338935755,29.0,18.617804338935755,327.0,5.0,0.0},{59.140671030107455,30.78,28.360671030107454,330.0,5.0,0.0},{48.46780433893575,30.76,17.70780433893575,333.0,5.0,0.0},{33.1293224884891,30.85,2.2793224884891004,336.0,5.0,0.0},{30.09,29.44,0.6499999999999986,339.0,5.0,0.0},{30.18,29.54,0.6400000000000006,342.0,5.0,0.0},{30.95,30.9,0.05000000000000071,345.0,5.0,0.0},{30.67,30.78,-0.10999999999999943,348.0,5.0,0.0},{29.03,30.16,-1.129999999999999,351.0,5.0,0.0},{29.93,29.98,-0.05000000000000071,354.0,5.0,0.0},{60.426340564892925,29.31,31.116340564892926,357.0,5.0,0.0},{78.8677850501791,29.43,49.437785050179095,360.0,5.0,0.0},{59.61634056489292,30.01,29.60634056489292,363.0,5.0,0.0},{36.35887081414851,30.41,5.94887081414851,366.0,5.0,0.0},{30.29,30.31,-0.019999999999999574,369.0,5.0,0.0},{30.79,29.68,1.1099999999999994,372.0,5.0,0.0},{30.86,29.12,1.7399999999999984,375.0,5.0,0.0},{30.38,30.95,-0.5700000000000003,378.0,5.0,0.0},{30.12,30.22,-0.09999999999999787,381.0,5.0,0.0},{29.14,29.09,0.05000000000000071,384.0,5.0,0.0},{29.21,29.91,-0.6999999999999993,387.0,5.0,0.0},{30.26,30.4,-0.13999999999999702,390.0,5.0,0.0},{30.21,30.76,-0.5500000000000007,393.0,5.0,0.0},{30.4,29.07,1.3299999999999983,396.0,5.0,0.0},{29.07,29.79,-0.7199999999999989,399.0,5.0,0.0},{29.97,29.53,0.4399999999999977,402.0,5.0,0.0},{29.75,30.18,-0.4299999999999997,405.0,5.0,0.0},{30.63,29.6,1.0299999999999976,408.0,5.0,0.0},{30.29,29.72,0.5700000000000003,411.0,5.0,0.0},{29.78,30.84,-1.0599999999999987,414.0,5.0,0.0},{30.95,30.34,0.6099999999999994,417.0,5.0,0.0},{29.14,29.81,-0.6699999999999982,420.0,5.0,0.0},{29.94,29.68,0.26000000000000156,423.0,5.0,0.0},{30.83,29.64,1.1899999999999977,426.0,5.0,0.0},{30.21,29.91,0.3000000000000007,429.0,5.0,0.0},{30.18,29.7,0.4800000000000004,432.0,5.0,0.0},{29.21,29.72,-0.509999999999998,435.0,5.0,0.0},{30.78,30.54,0.240000000000002,438.0,5.0,0.0},{30.7,29.77,0.9299999999999997,441.0,5.0,0.0},{29.5,30.96,-1.4600000000000009,444.0,5.0,0.0},{29.96,30.07,-0.10999999999999943,447.0,5.0,0.0},{29.32,30.14,-0.8200000000000003,450.0,5.0,0.0},{29.98,29.8,0.17999999999999972,453.0,5.0,0.0},{30.97,29.83,1.1400000000000006,456.0,5.0,0.0},{29.41,30.36,-0.9499999999999993,459.0,5.0,0.0},{30.87,29.48,1.3900000000000006,462.0,5.0,0.0},{29.04,29.74,-0.6999999999999993,465.0,5.0,0.0},{29.81,30.06,-0.25,468.0,5.0,0.0},{43.24050016153467,30.0,13.24050016153467,471.0,5.0,0.0},{47.09729587628938,29.32,17.77729587628938,474.0,5.0,0.0},{53.88232659798555,29.01,24.872326597985552,477.0,5.0,0.0},{58.9464869422972,30.46,28.486486942297198,477.0,10.578811355903323,4.049322488489104},{64.71298633137793,30.03,34.68298633137793,477.0,16.707598387510494,5.92126187256706},{69.97695234701595,30.53,39.44695234701595,477.0,25.481670640294453,8.319062600959166},{75.1924168363988,29.09,46.1024168363988,477.0,37.55184654370354,11.229559922680865},{76.65512327705471,30.5,46.15512327705471,477.0,53.50739665309435,14.563954123740972},{78.72307364817192,30.19,48.53307364817192,477.0,73.7746930781275,18.147804338935753},{77.01512327705471,29.56,47.45512327705471,477.0,98.51276034127943,21.726866457111203},{74.50241683639881,29.57,44.93241683639881,477.0,127.5276997537842,24.991845216884972},{69.17695234701596,29.34,39.83695234701595,477.0,160.2288901511992,27.620260522749252},{63.992986331377935,29.45,34.54298633137793,477.0,195.6442351108171,29.32820204815919},{59.6064869422972,30.56,29.046486942297204,477.0,232.5,29.920671030107453},{53.012326597985556,29.81,23.202326597985557,477.0,269.35576488918286,29.32820204815919},{47.15729587628938,29.4,17.757295876289383,477.0,304.7711098488008,27.620260522749252},{43.980500161534664,30.76,13.220500161534662,477.0,337.4723002462158,24.991845216884972},{39.5840189961073,29.07,10.514018996107296,477.0,366.48723965872057,21.726866457111203},{36.25891598158257,30.87,5.388915981582567,477.0,391.2253069218725,18.147804338935753},{33.80695114154777,29.57,4.23695114154777,477.0,411.49260334690564,14.563954123740972},{32.13734363538115,30.28,1.8573436353811488,477.0,427.44815345629644,11.229559922680865},{31.689956308042273,30.74,0.9499563080422746,477.0,439.51832935970555,8.319062600959166},{29.949854189957595,30.62,-0.6701458100424063,477.0,448.29240161248947,5.92126187256706},{30.34182180943256,29.88,0.46182180943256057,477.0,454.4211886440967,4.049322488489104},{29.76609058417578,29.93,-0.16390941582421803,477.0,458.5348969062232,2.6605944634673566},{29.47786630021676,30.23,-0.75213369978324,477.0,461.1881457950628,1.6795897721132176},{30.143428231613655,30.94,-0.7965717683863467,477.0,462.83254756897077,1.018722692526421},{30.585033631094976,29.92,0.6650336310949747,477.0,463.811864396351,0.5936588687234977},{30.456059627091786,29.92,0.5360596270917846,477.0,464.372297415292,0.3323886308953506},{30.117073168130784,30.89,-0.7729268318692171,477.0,464.68048085886915,0.17880661510986304},{29.702993096554806,29.95,-0.24700690344519316,477.0,464.84332789146026,0.09241643763547648},{29.271216902247858,30.41,-1.1387830977521425,477.0,464.92601450557675,0.04589264475853289},{29.860475355890923,30.9,-1.0395246441090755,477.0,464.96635815957484,0.02189601943435952},{30.51,29.93,0.5800000000000018,477.0,464.9852728725476,0.010037266932366402},{29.29,29.72,-0.4299999999999997,477.0,464.9937942267076,0.00442073008174049},{29.66,29.79,-0.129999999999999,477.0,464.9974831670829,0.001870685346754015},{30.44,29.03,1.4100000000000001,477.0,464.9990177085633,0.0007605639049115071},{30.29,29.84,0.4499999999999993,477.0,464.99963110240935,0.00029709743182740567},{30.65,30.44,0.2099999999999973,477.0,464.99986670701907,0.00011150396360507231},{30.12,30.0,0.120000000000001,477.0,464.9999536654176,0.0},{30.19,29.47,0.7200000000000024,477.0,464.99998450599145,0.0},{30.75,29.53,1.22,477.0,464.99999501632055,0.0},{30.97,29.99,0.98,477.0,464.9999984581781,0.0}};
    std::map<QString, double> curr_data;
    for (int i=0; i<200; i++){
        curr_data["Time"] = (double) i /3;
        curr_data["PBLAD"]= data[i][0];
        curr_data["PABD"]= data[i][1];
        curr_data["PDET"]= data[i][2];
        curr_data["VI"]= data[i][3];
        curr_data["VV"]= data[i][4];
        curr_data["Q"]= data[i][5];
        ui->valueDisplay->updateNumbers(curr_data);
        ui->graphDisplay->addDataset(curr_data);
    }*/

}

void MainWindow::recieveStopRecordingCSV(){
    emit sendEnterRecording(false);
    data_writer.setEndCSVRecording();
    write_to_csv=data_writer.checkCSVCreated();
}

void MainWindow::recieveEventCode(int code){
    event_code = code;
    ui->graphDisplay->createEventLine(event_code);
    qDebug() << event_code;
}

void MainWindow::recieveZeroPressure(){data_reader.setZeroPressure();}

void MainWindow::recievePumpValue(int value){
    writeToSerialPort(value);
}
