#include "pumpcontrol.h"
#include "ui_pumpcontrol.h"

#include "AppConstants.h"

PumpControl::PumpControl(QWidget* parent) : QFrame(parent),
                                            ui(new Ui::PumpControl) {
    ui->setupUi(this);
    initBar();
}

PumpControl::~PumpControl() {
    delete ui;
}

void PumpControl::initBar() {
    connect(ui->addOneButton, &QPushButton::clicked, this, &PumpControl::addOneToPump);
    connect(ui->addTenButton, &QPushButton::clicked, this, &PumpControl::addTenToPump);
    connect(ui->minusOneButton, &QPushButton::clicked, this, &PumpControl::minusOneToPump);
    connect(ui->minusTenButton, &QPushButton::clicked, this, &PumpControl::minusTenToPump);

    enablePump(false);
}

void PumpControl::enablePump(bool enable) {
    ui->flowrateLCD->display(0.0);
    ui->addOneButton->setEnabled(enable);
    ui->addTenButton->setEnabled(enable);
    ui->minusOneButton->setEnabled(enable);
    ui->minusTenButton->setEnabled(enable);
}

void PumpControl::resetPump() {
    pump_rate_arduino_input = PumpConstants::PUMP_INPUT_MIN;
}

void PumpControl::setTestingType(QString test) {
    if (test == TestTypeConstants::UDS_INVESTIGATION_DESC || test == TestTypeConstants::VOLUME_INFUSED_TEST_DESC || test == TestTypeConstants::INFUSION_RATE_TEST_DESC) {
        enablePump(true);
    }
}

void PumpControl::setExitTestingType() {
    resetPump();
    enablePump(false);
}

void PumpControl::addOneToPump() {
    pump_rate_arduino_input++;
    if (pump_rate_arduino_input > PumpConstants::PUMP_INPUT_MAX) {
        pump_rate_arduino_input = PumpConstants::PUMP_INPUT_MAX;
    }
    emit sendPumpRate(pump_rate_arduino_input);
}

void PumpControl::addTenToPump() {
    pump_rate_arduino_input = pump_rate_arduino_input + 10;
    if (pump_rate_arduino_input > PumpConstants::PUMP_INPUT_MAX) {
        pump_rate_arduino_input = PumpConstants::PUMP_INPUT_MAX;
    }
    emit sendPumpRate(pump_rate_arduino_input);
}

void PumpControl::minusOneToPump() {
    pump_rate_arduino_input--;
    if (pump_rate_arduino_input < PumpConstants::PUMP_INPUT_MIN) {
        pump_rate_arduino_input = PumpConstants::PUMP_INPUT_MIN;
    }
    emit sendPumpRate(pump_rate_arduino_input);
}

void PumpControl::minusTenToPump() {
    pump_rate_arduino_input = pump_rate_arduino_input - 10;
    if (pump_rate_arduino_input < PumpConstants::PUMP_INPUT_MIN) {
        pump_rate_arduino_input = PumpConstants::PUMP_INPUT_MIN;
    }
    emit sendPumpRate(pump_rate_arduino_input);
}

int PumpControl::getPumpArduinoInput() {
    return pump_rate_arduino_input;
}

void PumpControl::setPumpButtonsEnabled(bool enabled) {
    ui->addOneButton->setEnabled(enabled);
    ui->addTenButton->setEnabled(enabled);
    ui->minusOneButton->setEnabled(enabled);
    ui->minusTenButton->setEnabled(enabled);
}

void PumpControl::setPumpInfusionRate(double infusion_rate) {
    ui->flowrateLCD->display(infusion_rate);
}
