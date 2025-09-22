#include "investigationbar.h"
#include "ui_investigationbar.h"

#include "AppConstants.h"
#include <QDebug>

InvestigationBar::InvestigationBar(QWidget* parent) : QFrame(parent),
                                                      ui(new Ui::InvestigationBar) {
    ui->setupUi(this);
    initBar();
}

InvestigationBar::~InvestigationBar() {
    delete ui;
}

// INITIALISATION

void InvestigationBar::initBar() {

    ui->newPatientButton->setMaximumWidth(DimensionConstants::MAX_CONTROL_BUTTON_WIDTH);
    ui->existingPatientButton->setMaximumWidth(DimensionConstants::MAX_CONTROL_BUTTON_WIDTH);
    ui->recordButton->setMaximumWidth(DimensionConstants::MAX_CONTROL_BUTTON_WIDTH);
    ui->zeroPressureButton->setMaximumWidth(DimensionConstants::MAX_CONTROL_BUTTON_WIDTH);
    ui->sensationsButton->setMaximumWidth(DimensionConstants::MAX_CONTROL_BUTTON_WIDTH);

    // connections
    connect(ui->newPatientButton, &QPushButton::clicked, this, &InvestigationBar::openNewPatient);
    connect(ui->existingPatientButton, &QPushButton::clicked, this, &InvestigationBar::openExistingPatient);
    connect(ui->recordButton, &QPushButton::clicked, this, &InvestigationBar::recordInvestigationControl);
    connect(ui->zeroPressureButton, &QPushButton::clicked, this, &InvestigationBar::setZeroPressure);
    connect(ui->sensationsButton, &QPushButton::clicked, this, &InvestigationBar::logSensation);

    // appearance
    ui->newPatientButton->setCheckable(true);
    ui->existingPatientButton->setCheckable(true);
    ui->recordButton->setCheckable(true);

    resetView();
}

void InvestigationBar::resetView() {

    ui->newPatientButton->setVisible(true);
    ui->newPatientButton->setEnabled(false);

    ui->existingPatientButton->setVisible(false);
    ui->existingPatientButton->setEnabled(false);

    ui->recordButton->setEnabled(false);

    ui->zeroPressureButton->setVisible(true);
    ui->zeroPressureButton->setEnabled(false);

    ui->sensationsButton->setVisible(false);
    ui->sensationsButton->setEnabled(false);
    ui->sensationsButton->setText(EventConstants::FIRST_DESIRE_DESC);
    sensation_current_event_code = EventConstants::FIRST_DESIRE;
}

// TEST SELECTION

void InvestigationBar::recieveTestingType(QString test) {
    test_type = test;
    if (test_type == TestTypeConstants::UDS_INVESTIGATION_DESC) {
        ui->sensationsButton->setVisible(true);

    } else if (test_type == TestTypeConstants::PRESSURE_TEST_DESC) {
        ui->newPatientButton->setVisible(false);
        ui->recordButton->setEnabled(true);

    } else if (test_type == TestTypeConstants::VOLUME_VOID_TEST_DESC) {
        ui->newPatientButton->setVisible(false);
        ui->recordButton->setEnabled(true);

    } else if (test_type == TestTypeConstants::VOLUME_INFUSED_TEST_DESC) {
        ui->newPatientButton->setVisible(false);
        ui->recordButton->setEnabled(true);

    } else if (test_type == TestTypeConstants::INFUSION_RATE_TEST_DESC) {
        ui->newPatientButton->setVisible(false);
        ui->recordButton->setEnabled(true);
    }
}

void InvestigationBar::setExitTestingType() {
    resetView();
    qDebug() << "this ran";
}

// SERIAL CONNECTION

void InvestigationBar::recieveSerialConnectionMade(bool connection_made) {
    ui->newPatientButton->setEnabled(connection_made);
    ui->zeroPressureButton->setEnabled(connection_made);
}

// PATIENT ENTERED

void InvestigationBar::setPatientInfoEntered(bool set) {
    ui->recordButton->setEnabled(set);
}

// RECORD INVESTIGATION

void InvestigationBar::recordInvestigationControl() {
    bool investigation_started = ui->recordButton->isChecked();

    if (investigation_started) {
        ui->recordButton->setText("Stop");
        emit sendStartRecording();
    } else {
        ui->recordButton->setText("Start");
        emit sendStopRecording();
    }

    ui->zeroPressureButton->setEnabled(!investigation_started);
    ui->newPatientButton->setEnabled(!investigation_started);

    if (test_type == TestTypeConstants::UDS_INVESTIGATION_DESC) {
        ui->sensationsButton->setEnabled(investigation_started);

    } else if (test_type == TestTypeConstants::PRESSURE_TEST_DESC) {
        ui->zeroPressureButton->setEnabled(!investigation_started);

    } else if (test_type == TestTypeConstants::VOLUME_VOID_TEST_DESC) {
        ui->zeroPressureButton->setEnabled(!investigation_started);

    } else if (test_type == TestTypeConstants::VOLUME_INFUSED_TEST_DESC) {
        ui->zeroPressureButton->setEnabled(!investigation_started);

    } else if (test_type == TestTypeConstants::INFUSION_RATE_TEST_DESC) {
        ui->zeroPressureButton->setEnabled(!investigation_started);
    }
}

// BUTTON FUNCTIONALITY

void InvestigationBar::openNewPatient() {
    bool open = ui->newPatientButton->isChecked();
    // ui->existingPatientButton->setVisible(!open);

    if (open) {
        ui->newPatientButton->setText("Exit Patient");
        emit sendOpenNewPatient();

    } else {
        ui->newPatientButton->setText("New Patient");
        emit sendClosePatient();
    }
}

void InvestigationBar::openExistingPatient() {
    bool open = ui->existingPatientButton->isChecked();
    ui->newPatientButton->setVisible(!open);
    if (open) {
        ui->newPatientButton->setText("Exit Patient");
        emit sendOpenExistingPatient();

    } else {
        ui->newPatientButton->setText("Existing Patient");
        emit sendClosePatient();
    };
}

void InvestigationBar::setZeroPressure() {
    emit sendZeroPressure();
}

void InvestigationBar::logSensation() {

    switch (sensation_current_event_code) {
    case EventConstants::FIRST_DESIRE:
        emit sendEvent(EventConstants::FIRST_DESIRE);
        ui->sensationsButton->setText(EventConstants::NORMAL_DESIRE_DESC);
        sensation_current_event_code = EventConstants::NORMAL_DESIRE;
        break;
    case EventConstants::NORMAL_DESIRE:
        emit sendEvent(EventConstants::NORMAL_DESIRE);
        ui->sensationsButton->setText(EventConstants::STRONG_DESIRE_DESC);
        sensation_current_event_code = EventConstants::STRONG_DESIRE;
        break;
    case EventConstants::STRONG_DESIRE:
        emit sendEvent(EventConstants::STRONG_DESIRE);
        ui->sensationsButton->setText(EventConstants::URGENCY_DESC);
        sensation_current_event_code = EventConstants::URGENCY;
        break;
    case EventConstants::URGENCY:
        emit sendEvent(EventConstants::URGENCY);
        break;
    }
}

/*
 *
void InvestigationBar::setTestView(){
    ui->newPatientButton->setVisible(false);
    ui->existingPatientButton->setVisible(false);
    ui->recordButton->setEnabled(true);
}

void InvestigationBar::setUDSView(){
    ui->newPatientButton->setEnabled(true);
    ui->existingPatientButton->setEnabled(true);
    ui->sensationsButton->setVisible(true);
}

*/
