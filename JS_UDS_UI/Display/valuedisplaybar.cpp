#include "valuedisplaybar.h"
#include "qpen.h"
#include "ui_valuedisplaybar.h"

#include "AppConstants.h"

#include <QDebug>

ValueDisplayBar::ValueDisplayBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ValueDisplayBar)
{
    ui->setupUi(this);
    initDisplays();

    //
    ui->timeLabel->setVisible(false);
    ui->timeNumber->setVisible(false);

}

ValueDisplayBar::~ValueDisplayBar()
{
    delete ui;
}

void ValueDisplayBar::initDisplays(){
    display_labels.insert(0, ui->displayLabel1);
    display_labels.insert(1, ui->displayLabel2);
    display_labels.insert(2, ui->displayLabel3);
    display_labels.insert(3, ui->displayLabel4);
    display_labels.insert(4, ui->displayLabel5);
    display_labels.insert(5, ui->displayLabel6);
    display_labels.insert(6, ui->displayLabel7);

    display_numbers.insert(0,ui->displayNumber1);
    display_numbers.insert(1,ui->displayNumber2);
    display_numbers.insert(2,ui->displayNumber3);
    display_numbers.insert(3,ui->displayNumber4);
    display_numbers.insert(4,ui->displayNumber5);
    display_numbers.insert(5,ui->displayNumber6);
    display_numbers.insert(6,ui->displayNumber7);

    for (int i=0; i<display_labels.length(); i++){
        display_labels[i]->setText("-");
        display_numbers[i]->display(0.0);
        display_numbers[i]->setDecMode();
        display_numbers[i]->setSmallDecimalPoint(true);
        display_labels[i]->setVisible(false);
        display_numbers[i]->setVisible(false);

    }

}

void ValueDisplayBar::displayReset(){
    initDisplays();
}

void ValueDisplayBar::setTestingType(QString test){

    if (test == TestTypeConstants::UDS_INVESTIGATION_DESC){
        variables = UDSConsts::VARS_PLOT;

    }  else if (test == TestTypeConstants::PRESSURE_TEST_DESC){
        variables = PressureConsts::VARS_PLOT;

    } else if (test == TestTypeConstants::VOLUME_VOID_TEST_DESC){
        variables = VolumeVoidConsts::VARS_PLOT;

    }  else if (test == TestTypeConstants::VOLUME_INFUSED_TEST_DESC){
        variables = VolumeInfusedConsts::VARS_PLOT;

    }  else if (test == TestTypeConstants::INFUSION_RATE_TEST_DESC){
        variables = InfusionRateConsts::VARS_PLOT;
    }

    no_display_names = variables.length();

    for (int i=0; i<no_display_names; i++){
        display_labels[i]->setVisible(true);
        display_labels[i]->setText(variables[i]);
        display_numbers[i]->setVisible(true);
        display_numbers[i]->display(0.0);
    }

}

/*
void ValueDisplayBar::setDisplayChannels(QVector<QString> display_names){
    // set number of channels
    no_display_names = display_names.length();

    // link measured variables to the display
    for (int i=1; i<no_display_names; i++){
        // set mapping
        variable_mapping[display_names[i]] = i-1;

        // manage components
        display_labels[i-1]->setVisible(true);
        display_labels[i-1]->setText(display_names[i]);
        display_numbers[i-1]->setVisible(true);
        display_numbers[i-1]->display(0.0);
    }
}*/


void ValueDisplayBar::updateNumbers(std::map<QString, double> data){
    for (int i=0; i<no_display_names; i++){
        display_numbers[i]->display(data[variables[i]]);
    }
    ui->timeNumber->display(data[TIME_LAB]);
}
