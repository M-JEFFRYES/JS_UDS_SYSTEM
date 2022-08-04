#include "readdata.h"

#include <QDebug>

ReadData::ReadData()
{

}

// FUNCTIONALITY
void ReadData::setTestingType(QString testing_type){
    qInfo() << "Setting type" << testing_type;

    QVector<QString> names;
    QVector<QVector<double>> ranges;
    if (testing_type == "Pressure"){
        names = {"Time", "PSENS"};
        ranges = {{0.0, 100.0}};
        setChannelNames(names);
        setChannelRanges(ranges);

    }  else if (testing_type == "Uroflowmetry"){
        names = {"Time", "UF_RAW"};
        ranges = {{0.0, 500.0}};
        setChannelNames(names);
        setChannelRanges(ranges);

    }  else if (testing_type == "Volume Infused"){
        names = {"Time", "VI_RAW"};
        ranges = {{0.0, 500.0}};
        setChannelNames(names);
        setChannelRanges(ranges);

    }  else if (testing_type == "Pump"){
        names = {"Time", "PWM", "UF_RAW"};
        ranges = {{0.0, 255.0}, {0.0, 500.0}};
        setChannelNames(names);
        setChannelRanges(ranges);

    }  else if (testing_type == "UDS Investigation"){
        names = {"Time", "PBLAD", "PABD", "PDET","VI", "VV", "Q"};
        /*ranges.append({0.0, 100.0});
        ranges.append({0.0, 100.0});
        ranges.append({0.0, 100.0});
        ranges.append({0.0, 500.0});
        ranges.append({0.0, 500.0});
        ranges.append({0.0, 50.0});
        */
        ranges = {{0.0, 100.0}, {0.0, 100.0}, {0.0, 100.0}, {0.0, 500.0}, {0.0, 500.0}, {0.0, 50.0}};
        setChannelNames(names);
        setChannelRanges(ranges);
    }
}

void ReadData::setChannelNames(QVector<QString> names){
    channel_names.clear();
    number_data_channels = names.length();
    for (int i=0; i<number_data_channels; i++){
        channel_names[i] = names[i];
        channel_zeros[names[i]] = 0.0;
    }
}

void ReadData::setChannelRanges(QVector<QVector<double>> ranges){
    channel_ranges = ranges;
}



void ReadData::readSerialData(QString data_string, int event, bool zero_sensors){
    QStringList values = data_string.split(",");
    double value;
    QString variable;
    for (int i=0; i<number_data_channels; i++){
        variable = channel_names[i];
        value = values.value(i).toDouble();
        if (zero_sensors){
            channel_zeros[variable] = value;
        }
        value -= channel_zeros[variable];
        current_dataset[variable] = value;
    }
}

std::map<int, QString> ReadData::getChannelNames(){
    return channel_names;
}

QVector<QVector<double>> ReadData::getChannelRanges(){
    return channel_ranges;
}

std::map<QString, double> ReadData::getChannelZeros(){
    return channel_zeros;
}

// SLOTS

std::map<QString, double> ReadData::readCurrentDataset(QString data_string, int event, bool zero_channels){
    readSerialData(data_string,event, zero_channels);
    return current_dataset;
}

