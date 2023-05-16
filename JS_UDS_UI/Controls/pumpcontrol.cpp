#include "pumpcontrol.h"
#include "ui_pumpcontrol.h"

PumpControl::PumpControl(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PumpControl)
{
    ui->setupUi(this);
    initBar();
}

PumpControl::~PumpControl()
{
    delete ui;
}

void PumpControl::initBar(){
    pump_rate_percentage = 0;
    connect(ui->addOneButton, &QPushButton::clicked, this, &PumpControl::addOneToPump);
    connect(ui->addTenButton, &QPushButton::clicked, this, &PumpControl::addTenToPump);
    connect(ui->minusOneButton, &QPushButton::clicked, this, &PumpControl::minusOneToPump);
    connect(ui->minusTenButton, &QPushButton::clicked, this, &PumpControl::minusTenToPump);
}

void PumpControl::resetPump(){
    pump_rate_percentage = 0;
}

void PumpControl::convertToFlowrate(){
    pump_rate_arduino_input = (int) (((float) pump_rate_percentage / 100.0) * 255.0);
}

void PumpControl::addOneToPump(){
    pump_rate_percentage++;
    if (pump_rate_percentage > 100) {
        pump_rate_percentage = 100;
    }
    convertToFlowrate();
    emit sendPumpRate(pump_rate_arduino_input);
}

void PumpControl::addTenToPump(){
    pump_rate_percentage = pump_rate_percentage + 10;
    if (pump_rate_percentage > 100) {
        pump_rate_percentage = 100;
    }
    convertToFlowrate();
    emit sendPumpRate(pump_rate_arduino_input);
}

void PumpControl::minusOneToPump(){
    pump_rate_percentage--;
    if (pump_rate_percentage < 0) {
        pump_rate_percentage = 0;
    }
    convertToFlowrate();
    emit sendPumpRate(pump_rate_arduino_input);
}

void PumpControl::minusTenToPump(){
    pump_rate_percentage = pump_rate_percentage - 10;
    if (pump_rate_percentage < 0) {
        pump_rate_percentage = 0;
    }
    convertToFlowrate();
    emit sendPumpRate(pump_rate_arduino_input);
}

int PumpControl::getPumpPower(){return pump_rate_percentage;}

int PumpControl::getPumpArduinoInput(){return pump_rate_arduino_input;}
