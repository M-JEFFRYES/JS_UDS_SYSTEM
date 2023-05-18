#include "investigationbar.h"
#include "ui_investigationbar.h"

#include "AppConstants.h"

InvestigationBar::InvestigationBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::InvestigationBar)
{
    ui->setupUi(this);
    initBar();
}

InvestigationBar::~InvestigationBar()
{
    delete ui;
}

void InvestigationBar::initBar(){

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

void InvestigationBar::resetView(){

    ui->newPatientButton->setVisible(true);
    ui->newPatientButton->setEnabled(false);

    ui->existingPatientButton->setVisible(true);
    ui->existingPatientButton->setEnabled(false);

    ui->recordButton->setEnabled(false);

    ui->zeroPressureButton->setVisible(true);

    ui->sensationsButton->setVisible(false);
    ui->sensationsButton->setText(EventConstants::FIRST_DESIRE_DESC);
    sensation_current_event_code = EventConstants::FIRST_DESIRE;
}

void InvestigationBar::setInvestigationView(int test_code){
    switch (test_code){
        case TestTypeConstants::UDS_INVESTIGATION:
            ui->newPatientButton->setEnabled(true);
            ui->existingPatientButton->setEnabled(true);
            ui->sensationsButton->setVisible(true);
            break;
        case TestTypeConstants::PRESSURE_TEST:
        case TestTypeConstants::VOLUME_VOID_TEST:
        case TestTypeConstants::VOLUME_INFUSED_TEST:
        case TestTypeConstants::INFUSION_RATE_TEST:
            ui->newPatientButton->setVisible(false);
            ui->existingPatientButton->setVisible(false);
            ui->recordButton->setEnabled(true);
            break;
    }
}


void InvestigationBar::setUDSView(){
    ui->newPatientButton->setEnabled(true);
    ui->existingPatientButton->setEnabled(true);
    ui->sensationsButton->setVisible(true);
}

void InvestigationBar::setUDSReadyView(bool set){
    ui->recordButton->setEnabled(set);
}

void InvestigationBar::setTestView(){
    ui->newPatientButton->setVisible(false);
    ui->existingPatientButton->setVisible(false);
    ui->recordButton->setEnabled(true);
}

void InvestigationBar::openNewPatient()
{
    bool open = ui->newPatientButton->isChecked();
    ui->existingPatientButton->setVisible(!open);

    if (open){
        ui->newPatientButton->setText("Exit Patient");
        emit sendOpenNewPatient();

    } else {
        ui->newPatientButton->setText("New Patient");
        emit sendClosePatient();
    }

}

void InvestigationBar::openExistingPatient()
{
    bool open = ui->existingPatientButton->isChecked();
    ui->newPatientButton->setVisible(!open);
    if (open){
        ui->newPatientButton->setText("Exit Patient");
        emit sendOpenExistingPatient();

    } else {
        ui->newPatientButton->setText("Existing Patient");
        emit sendClosePatient();
    }
   ;
}

void InvestigationBar::recordInvestigationControl()
{
    if (ui->recordButton->isChecked()){
        ui->recordButton->setText("Stop");
        ui->newPatientButton->setEnabled(false);
        ui->existingPatientButton->setEnabled(false);
        ui->zeroPressureButton->setVisible(false);

        ui->sensationsButton->setEnabled(true);

        emit sendStartRecording();

    } else {
        ui->recordButton->setText("Start");
        ui->newPatientButton->setEnabled(true);
        ui->existingPatientButton->setEnabled(true);
        ui->zeroPressureButton->setVisible(true);
        emit sendStopRecording();
    }
}

void InvestigationBar::setZeroPressure(){emit sendZeroPressure();}

void InvestigationBar::logSensation(){

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




