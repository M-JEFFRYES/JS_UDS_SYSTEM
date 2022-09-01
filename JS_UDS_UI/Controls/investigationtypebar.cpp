#include "investigationtypebar.h"
#include "ui_investigationtypebar.h"

InvestigationTypeBar::InvestigationTypeBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::InvestigationTypeBar)
{
    ui->setupUi(this);
    ui->selectTestButton->setCheckable(true);
    setTestOptions();
}

InvestigationTypeBar::~InvestigationTypeBar()
{
    delete ui;
}

// FUNCTIONALITY
void InvestigationTypeBar::setTestOptions(){
    ui->testsBox->addItem("Clinical - UDS Investigation");
    ui->testsBox->addItem("Test - Pressure");
    ui->testsBox->addItem("Test - Uroflowmetry");
    ui->testsBox->addItem("Test - Volume Infused");
    ui->testsBox->addItem("Test - Pump");
}

void InvestigationTypeBar::setTestSelected(){
    current_test_type = ui->testsBox->currentText().split(" - ")[1];
    ui->selectTestButton->setText("Exit Test");
    ui->testsBox->setEnabled(false);
    emit sendTestName(current_test_type);
}

void InvestigationTypeBar::exitTestSelected(){
    current_test_type = "";
    ui->selectTestButton->setText("Select Investigation Type");
    ui->testsBox->setEnabled(true);
    emit sendExitTest();
}

void InvestigationTypeBar::setConnectionOpen(bool open){
    ui->selectTestButton->setEnabled(!open);
}


// OBJECTS
QString InvestigationTypeBar::getSelectedTest(){
    return current_test_type;
}

// SLOTS
void InvestigationTypeBar::on_selectTestButton_clicked()
{
    if (ui->selectTestButton->isChecked()){
        setTestSelected();
        emit sendTestSelected(true);
    }
    else {
        exitTestSelected();
        emit sendTestSelected(false);
    }
}

void InvestigationTypeBar::receiveConnectionOpen(bool open){
     setConnectionOpen(open);
}
