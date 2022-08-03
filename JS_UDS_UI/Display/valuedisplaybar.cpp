#include "valuedisplaybar.h"
#include "ui_valuedisplaybar.h"

ValueDisplayBar::ValueDisplayBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ValueDisplayBar)
{
    ui->setupUi(this);
    initDisplays();
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
        display_labels[i]->setVisible(false);
        display_numbers[i]->setVisible(false);
    }
}

void ValueDisplayBar::setDisplayChannels(std::map<int, QString> display_names){

    this->display_names = display_names;
    no_display_names = this->display_names.size();

    for (int i=0; i<no_display_names; i++){
        display_labels[i]->setVisible(true);
        display_numbers[i]->setVisible(true);
        display_labels[i]->setText(display_names[i]);
        variable_mapping[display_names[i]] = i;
        display_numbers[i]->display(0.0);
    }
}

void ValueDisplayBar::displayReset(){
    initDisplays();
}

void ValueDisplayBar::updateNumbers(std::map<QString, double> data){
    for (int i=0; i<no_display_names; i++){
        int pos = variable_mapping[display_names[i]];
        double val = data[display_names[i]];
        display_numbers[pos]->display(val);
    }
}
