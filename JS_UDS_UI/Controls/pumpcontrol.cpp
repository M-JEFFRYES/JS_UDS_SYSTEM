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

    ui->addOneButton->setEnabled(false);
    ui->addTenButton->setEnabled(false);
    ui->minusOneButton->setEnabled(false);
    ui->minusTenButton->setEnabled(false);
}

void PumpControl::resetPump(){
    pump_rate_arduino_input = 0;
}

void PumpControl::addOneToPump(){
    pump_rate_arduino_input++;
    if (pump_rate_arduino_input > 255) {
        pump_rate_arduino_input = 255;
    }
    emit sendPumpRate(pump_rate_arduino_input);
}

void PumpControl::addTenToPump(){
    pump_rate_arduino_input = pump_rate_arduino_input + 10;
    if (pump_rate_arduino_input > 255) {
        pump_rate_arduino_input = 255;
    }
    emit sendPumpRate(pump_rate_arduino_input);
}

void PumpControl::minusOneToPump(){
    pump_rate_arduino_input--;
    if (pump_rate_arduino_input < 0) {
        pump_rate_arduino_input = 0;
    }
    emit sendPumpRate(pump_rate_arduino_input);
}

void PumpControl::minusTenToPump(){
    pump_rate_arduino_input = pump_rate_arduino_input - 10;
    if (pump_rate_arduino_input < 0) {
        pump_rate_arduino_input = 0;
    }
    emit sendPumpRate(pump_rate_arduino_input);
}

int PumpControl::getPumpArduinoInput(){return pump_rate_arduino_input;}

void PumpControl::setPumpButtonsEnabled(bool enabled){
    ui->addOneButton->setEnabled(enabled);
    ui->addTenButton->setEnabled(enabled);
    ui->minusOneButton->setEnabled(enabled);
    ui->minusTenButton->setEnabled(enabled);
}

void PumpControl::setPumpInfusionRate(double infusion_rate){
    ui->flowrateLCD->display(infusion_rate);
}
