#include "readdata.h"

#include <QDebug>

ReadData::ReadData()
{

}

// GETTERS
std::map<int, QString> ReadData::getChannelNames(){
    return channel_names;
}

QVector<QString> ReadData::getChannelVarNames(){
    return channel_var_names;
}

QVector<QVector<double>> ReadData::getChannelRanges(){
    return channel_ranges;
}

std::map<QString, double> ReadData::getChannelZeros(){
    return channel_zeros;
}

std::map<QString, int> ReadData::getChannelPlotNumbers(){
    return channel_plot_numbers;
}

QVector<QString> ReadData::getEventCodes(){return event_codes;}

QVector<QString> ReadData::getEventLabels(){return event_labels;}

// FUNCTIONALITY
void ReadData::setTestingType(QString testing_type){
    qInfo() << "Setting type" << testing_type;
    test_type = testing_type;
    QVector<QString> names;
    QVector<QVector<double>> ranges;
    QVector<int> plot_no;
    QVector<QString> event_codes;
    QVector<QString> event_labels;

    if (test_type == "Pressure"){
        names = {"Time", "PSENS"};
        plot_no = {0};
        ranges = {{0.0, 100.0}};
        event_codes = {"First Height", "Second", "Third"};
        event_labels = {"H1", "H2", "H3"};
        setChannelNamesRangesEvents(names, ranges, event_codes, event_labels, plot_no);

    }  else if (test_type == "Uroflowmetry"){
        names = {"Time", "UF"};
        plot_no = {0};
        ranges = {{0.0, 500.0}};
        event_codes = {"First Height", "Second", "Third"};
        event_labels = {"H1", "H2", "H3"};
        setChannelNamesRangesEvents(names, ranges, event_codes, event_labels, plot_no);

    }  else if (test_type == "Volume Infused"){
        names = {"Time", "VI"};
        plot_no = {0};
        ranges = {{0.0, 500.0}};
        event_codes = {"First Height", "Second", "Third"};
        event_labels = {"H1", "H2", "H3"};
        setChannelNamesRangesEvents(names, ranges, event_codes, event_labels, plot_no);

    }  else if (test_type == "Pump"){
        names = {"Time", "PWM", "UF"};
        plot_no = {0, 1};
        ranges = {{0.0, 255.0}, {0.0, 500.0}};
        event_codes = {"First Height", "Second", "Third"};
        event_labels = {"H1", "H2", "H3"};
        setChannelNamesRangesEvents(names, ranges, event_codes, event_labels, plot_no);

    }  else if (test_type == "UDS Investigation"){
        names = {"Time", "PBLAD", "PABD", "PDET","VI", "VV", "Q"};
        plot_no = {0, 1, 2, 3, 3, 4};
        ranges = {{0.0, 100.0}, {0.0, 100.0}, {0.0, 100.0}, {0.0, 500.0}, {0.0, 500.0}, {0.0, 50.0}};
        event_codes = {"DO", "Leak", "Valsalva"};
        event_labels = {"do", "lk", "ch"};
        setChannelNamesRangesEvents(names, ranges, event_codes, event_labels, plot_no);
    }
}

void ReadData::setChannelNamesRangesEvents(QVector<QString> names, QVector<QVector<double>> ranges, QVector<QString> event_codes, QVector<QString> event_labels, QVector<int> channel_plot_numbers){
    channel_var_names = names;

    channel_names.clear();
    this->event_codes.clear();
    number_data_channels = names.length();
    for (int i=0; i<number_data_channels; i++){
        channel_names[i] = names[i];
        channel_zeros[names[i]] = 0.0;

    }
    this->event_codes = event_codes;
    this->event_labels = event_labels;
    channel_ranges = ranges;

    for (int i=1; i<number_data_channels; i++){
        this->channel_plot_numbers[channel_names[i]] =  channel_plot_numbers.at(i-1);
    }
}

void ReadData::readSerialData(QString data_string, int event, bool zero_sensors){

    QStringList valuePairs = data_string.split(";");
    QStringList keyPair;
    double value;
    QString variable;
    for (int i=0; i<valuePairs.length(); i++){
        keyPair = valuePairs[i].split(",");
        variable = keyPair[0];
        value = keyPair[1].toDouble();
        if (zero_sensors){
            channel_zeros[variable] = value;
        }
        value -= channel_zeros[variable];
        current_dataset[variable] = value;
    }
    current_dataset["EVENT"] = event;
}

void ReadData::setZeroPressure(){
    if (test_type == "UDS Investigation"){
        channel_zeros["PBLAD"] = current_dataset["PBLAD"];
        channel_zeros["PABD"] = current_dataset["PABD"];

    } else if (test_type == "Pressure") {
        channel_zeros["PSENS"] = current_dataset["PSENS"];
    }
}

// SLOTS

std::map<QString, double> ReadData::readCurrentDataset(QString data_string, int event, bool zero_channels){
    readSerialData(data_string,event, zero_channels);
    return current_dataset;
}

