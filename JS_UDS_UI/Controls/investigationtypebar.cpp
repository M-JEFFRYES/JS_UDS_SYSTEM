#include "investigationtypebar.h"
#include "ui_investigationtypebar.h"

InvestigationTypeBar::InvestigationTypeBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::InvestigationTypeBar)
{
    ui->setupUi(this);
    ui->endTestButton->setEnabled(false);
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

void InvestigationTypeBar::setTestSelected(bool selected){
    ui->selectTestButton->setEnabled(!selected);
    ui->testsBox->setEnabled(!selected);
    ui->endTestButton->setEnabled(selected);
}

// OBJECTS
QString InvestigationTypeBar::getSelectedTest(){
    return ui->testsBox->currentText();
}

// SLOTS
void InvestigationTypeBar::on_selectTestButton_clicked()
{
    if (ui->selectTestButton->text() != ""){
        QString test_type = ui->testsBox->currentText().split(" - ")[1];
        ui->selectTestButton->setText(test_type);
        setTestSelected(true);
        emit sendTestName(test_type);
    }
}


void InvestigationTypeBar::on_endTestButton_clicked()
{
    ui->selectTestButton->setText("Select Test");
    setTestSelected(false);
    emit sendExitTest();
}
