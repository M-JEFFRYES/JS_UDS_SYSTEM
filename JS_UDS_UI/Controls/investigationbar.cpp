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


void InvestigationBar::setView(QString test_type){

    QString e1_lab;
    QString e2_lab;
    QString e3_lab;

    if (test_type == "UDS Investigation"){
        setUDSView();

        e1_lab = "Leak";
        e2_lab = "DO";
        e3_lab = "E";

    } else {
        setTestView();

        e1_lab = "ESt";
        e2_lab = "ESp";
        e3_lab = "E";
    }

    ui->event1Button->setText(e1_lab);
    ui->event2Button->setText(e2_lab);
    ui->event3Button->setText(e3_lab);


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

void InvestigationBar::setUDSReadyView(){
    ui->recordButton->setEnabled(true);
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


void InvestigationBar::logEvent1()
{
    QString event = ui->event1Button->text();
    emit sendEvent(event);
}


void InvestigationBar::logEvent2()
{
    QString event = ui->event2Button->text();
    emit sendEvent(event);
}


void InvestigationBar::logEvent3()
{
    QString event = ui->event3Button->text();
    emit sendEvent(event);
}


