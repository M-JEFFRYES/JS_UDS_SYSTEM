#include "tracesdisplay.h"
#include "ui_tracesdisplay.h"

TracesDisplay::TracesDisplay(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TracesDisplay)
{
    ui->setupUi(this);
    initGraph();
}

TracesDisplay::~TracesDisplay()
{
    delete ui;
}


void TracesDisplay::initGraph(){
    ui->graph->plotLayout()->clear();
}

void TracesDisplay::initDataset(QVector<QString> channel_names, int no_samples){
    this->channel_names = channel_names;
    this->no_samples = no_samples;

    for (int var=0; var < channel_names.length(); var++){
        QVector<double> arr;
        for(int i=0; i<no_samples; i++){
            arr.insert(i, 0.0);
        }
        dataset.insert(var, arr);
    }
}





void TracesDisplay::loadCurrentDataset(std::map<QString, double> dataset){

}
