#include "investigationtypebar.h"
#include "ui_investigationtypebar.h"

#include "AppConstants.h"

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

    ui->testsBox->addItem(TestTypeConstants::UDS_INVESTIGATION_DESC);
    ui->testsBox->addItem(TestTypeConstants::PRESSURE_TEST_DESC);
    ui->testsBox->addItem(TestTypeConstants::VOLUME_VOID_TEST_DESC);
    ui->testsBox->addItem(TestTypeConstants::VOLUME_INFUSED_TEST_DESC);
    ui->testsBox->addItem(TestTypeConstants::INFUSION_RATE_TEST_DESC);
}

void InvestigationTypeBar::setTestSelected(){
    ui->selectTestButton->setText("Exit Test");
    ui->testsBox->setEnabled(false);
    current_test_type = ui->testsBox->currentText();
    emit sendTestType(current_test_type);
}

void InvestigationTypeBar::exitTestSelected(){
    current_test_type = "";
    ui->selectTestButton->setText("Select Investigation Type");
    ui->testsBox->setEnabled(true);
    emit sendExitTestType();
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
