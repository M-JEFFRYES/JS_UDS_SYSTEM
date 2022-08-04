#include "tracesdisplay.h"
#include "ui_tracesdisplay.h"

TracesDisplay::TracesDisplay(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TracesDisplay)
{
    ui->setupUi(this);
}

TracesDisplay::~TracesDisplay()
{
    delete ui;
}

void TracesDisplay::setChannelNames(std::map<int, QString> var_names, QVector<QVector<double>> var_ranges){
    no_channels = var_names.size();
    for (int i=1; i < (no_channels); i++){
        y_channel_names.append(var_names[i]);
    }

    qDebug() << 11;
    calculateOffetY();
    qDebug() << 2;
    calculateScalingY(var_ranges);
    qDebug() << 3;
    initDataset();
}

void TracesDisplay::calculateOffetY(){
    double y = 0;
    double offset = 10;
    QString var_name;

    var_name = y_channel_names.at(y_channel_names.length()-1);
    qDebug() << var_name;
    y_offsets[var_name] = y;
    for (int i=y_channel_names.length()-2; i>-1; i--){
        qDebug() << var_name;
        var_name = y_channel_names.at(i);
        y += offset;
        y_offsets[var_name] = y;
    }
    qDebug() << 2;
}

void TracesDisplay::calculateScalingY(QVector<QVector<double>> var_ranges){

    double actual_range = 100;
    double display_min;
    double display_max;
    double scale;
    QString name;
    QVector<double> vals;

    for (int i=0; i<y_channel_names.length(); i++){
        name = y_channel_names[i];
        vals = var_ranges.at(i);
        display_min = vals.at(0);
        display_max = vals.at(1);
        scale = actual_range/ (display_max - display_min);
        y_scales[name] = scale;
    }
}

void TracesDisplay::initDataset(){

    for(int i=0; i<no_samples; i++){
        time_dataset.insert(i, 0.0);
    }

    for (int var=0; var < y_channel_names.length(); var++){
        QVector<double> arr;
        double base_val;
        base_val = y_offsets[y_channel_names.at(var)];
        for(int i=0; i<no_samples; i++){
            arr.insert(i, base_val);
        }
        dataset.insert(var, arr);
    }
}






void TracesDisplay::addDataset(std::map<QString, double> dataset){
    std::map<QString, double> current_dataset = convertRawDataset(dataset);

}

std::map<QString, double> TracesDisplay::convertRawDataset(std::map<QString, double> dataset){

    QString name;
    std::map<QString, double> current_dataset;
    for (int i=0; i<y_channel_names.length(); i++){
        name = y_channel_names[i];
        current_dataset[name] = y_offsets[name] + (dataset[name] * y_scales[name]);
    }
    return current_dataset;
}



















void TracesDisplay::initGraph(){
    ui->graph->plotLayout()->clear();
}







void TracesDisplay::loadCurrentDataset(std::map<QString, double> dataset){

}
