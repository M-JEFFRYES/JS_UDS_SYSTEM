#include "connectionbar.h"
#include "ui_connectionbar.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

ConnectionBar::ConnectionBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ConnectionBar)
{
    ui->setupUi(this);
    initButtons();
}

ConnectionBar::~ConnectionBar()
{
    delete ui;
}

// INITIALISATION

void ConnectionBar::initButtons(){
    ui->connectButton->setCheckable(true);
    getOpenPorts();
    setTestSelected(false);
}

void ConnectionBar::setBarHeights(int minimum, int maximum){
    setMinimumHeight(minimum);
    setMaximumHeight(maximum);
}

bool ConnectionBar::isPortOpen(){
    return ui->connectButton->isChecked();
}

void ConnectionBar::getOpenPorts(){
    ui->portNamesBox->clear();
    for(QSerialPortInfo port : QSerialPortInfo::availablePorts())
            ui->portNamesBox->addItem(port.portName(),port.description());
    int ind = QSerialPortInfo::availablePorts().length() - 1;
    ui->portNamesBox->setCurrentIndex(ind); // Default value to most likely
}

// TEST SELECTION

void ConnectionBar::receiveTestSelected(bool selected){
    setTestSelected(selected);
}

void ConnectionBar::setTestSelected(bool selected){
    ui->connectButton->setEnabled(selected);
    ui->refreshButton->setEnabled(selected);
    ui->portNamesBox->setEnabled(selected);
}

// SERIAL CONNECTION

void ConnectionBar::setConnected(){
    ui->connectButton->setText("Disconnect");
    ui->refreshButton->setEnabled(false);
    ui->portNamesBox->setEnabled(false);
    emit sendSerialConnectionMade(true);
}

void ConnectionBar::setDisconnected(){
    ui->connectButton->setText("Connect");
    ui->refreshButton->setEnabled(true);
    ui->portNamesBox->setEnabled(true);
    emit sendSerialConnectionMade(false);
}

// TEST RECORDING

void ConnectionBar::setEnterRecordingMode(bool start){
    ui->connectButton->setEnabled(!start);
}


// RETURN OBJECTS

QPushButton* ConnectionBar::getConnectionButton(){
    return ui->connectButton;
}

QPushButton* ConnectionBar::getRefreshButton(){
    return ui->refreshButton;
}

QString ConnectionBar::getSelectedPort(){
    return ui->portNamesBox->currentText();
}

// SLOTS

void ConnectionBar::on_connectButton_clicked(){
    QString buttonText = ui->connectButton->text();
    if (buttonText == "Connect"){
        if (isPortOpen()){
            setConnected();
        }
    } else {
        setDisconnected();
    }
}

void ConnectionBar::on_refreshButton_clicked()
{
    getOpenPorts();
}



