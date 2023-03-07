#include "investigationbar.h"
#include "ui_investigationbar.h"

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
    connect(ui->newPatientButton, &QPushButton::clicked, this, &InvestigationBar::openNewPatient);
    connect(ui->existingPatientButton, &QPushButton::clicked, this, &InvestigationBar::openExistingPatient);
    connect(ui->recordButton, &QPushButton::clicked, this, &InvestigationBar::recordInvestigationControl);
    connect(ui->event1Button, &QPushButton::clicked, this, &InvestigationBar::logEvent1);
    connect(ui->event2Button, &QPushButton::clicked, this, &InvestigationBar::logEvent2);
    connect(ui->event3Button, &QPushButton::clicked, this, &InvestigationBar::logEvent3);


    ui->newPatientButton->setCheckable(true);
    ui->existingPatientButton->setCheckable(true);
    ui->recordButton->setCheckable(true);
    resetView();
}


void InvestigationBar::setView(QVector<QString> event_codes){

    ui->event1Button->setText(event_codes.at(0));
    ui->event2Button->setText(event_codes.at(1));
    ui->event3Button->setText(event_codes.at(2));

}

void InvestigationBar::resetView(){
    ui->newPatientButton->setVisible(true);
    ui->existingPatientButton->setVisible(true);

    ui->newPatientButton->setEnabled(false);
    ui->existingPatientButton->setEnabled(false);
    ui->recordButton->setEnabled(false);
    ui->event1Button->setEnabled(false);
    ui->event2Button->setEnabled(false);
    ui->event3Button->setEnabled(false);

    ui->event1Button->setText("Event 1");
    ui->event2Button->setText("Event 2");
    ui->event3Button->setText("Event 3");

}


void InvestigationBar::setUDSView(){
    ui->newPatientButton->setEnabled(true);
    ui->existingPatientButton->setEnabled(true);
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

        ui->event1Button->setEnabled(true);
        ui->event2Button->setEnabled(true);
        ui->event3Button->setEnabled(true);

        emit sendStartRecording();

    } else {
        ui->recordButton->setText("Start");
        ui->newPatientButton->setEnabled(true);
        ui->existingPatientButton->setEnabled(true);

        ui->event1Button->setEnabled(false);
        ui->event2Button->setEnabled(false);
        ui->event3Button->setEnabled(false);

        emit sendStopRecording();
    }
}


void InvestigationBar::logEvent1(){emit sendEvent(1);}
void InvestigationBar::logEvent2(){emit sendEvent(2);}
void InvestigationBar::logEvent3(){emit sendEvent(3);}


