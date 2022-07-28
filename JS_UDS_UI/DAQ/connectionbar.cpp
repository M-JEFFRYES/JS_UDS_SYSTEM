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

// FUNCTIONALITY

void ConnectionBar::initButtons(){
    ui->connectButton->setCheckable(true);
    getOpenPorts();
}

void ConnectionBar::setBarHeights(int minimum, int maximum){
    setMinimumHeight(minimum);
    setMaximumHeight(maximum);
}

void ConnectionBar::setConnected(){
    ui->connectButton->setText("Disconnect");
    ui->refreshButton->setEnabled(false);
    ui->portNamesBox->setEnabled(false);
}

void ConnectionBar::setDisconnected(){
    ui->connectButton->setText("Connect");
    ui->refreshButton->setEnabled(true);
    ui->portNamesBox->setEnabled(true);
}

void ConnectionBar::setEnterRecordingMode(bool start){
    if (start){
        ui->connectButton->setEnabled(false);
    } else {
        ui->connectButton->setEnabled(true);
    }
}

bool ConnectionBar::isPortOpen(){
    return ui->connectButton->isChecked();
}

void ConnectionBar::getOpenPorts(){
    ui->portNamesBox->clear();
    for(QSerialPortInfo port : QSerialPortInfo::availablePorts())
            ui->portNamesBox->addItem(port.portName(),port.description());
    ui->portNamesBox->setCurrentIndex(2); // Default value to most likely
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
