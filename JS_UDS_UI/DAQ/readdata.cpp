#include "readdata.h"

ReadData::ReadData()
{

}

// FUNCTIONALITY
void ReadData::setTestingType(QString testing_type){
    if (testing_type == "Pressure"){
        QVector<QString> names = {"Time", "PSENS"};
        setChannelNames(names);

    }  else if (testing_type == "Uroflowmetry"){
        QVector<QString> names = {"Time", "UF_RAW"};
        setChannelNames(names);

    }  else if (testing_type == "Volume Infused"){
        QVector<QString> names = {"Time", "VI_RAW"};
        setChannelNames(names);

    }  else if (testing_type == "Pump"){
        QVector<QString> names = {"Time", "PWM", "UF_RAW"};
        setChannelNames(names);

    }  else if (testing_type == "UDS Investigation"){
        QVector<QString> names = {"Time", "PBLAD", "PABD", "PDET","VI", "VV", "Q"};
        setChannelNames(names);
    }
}

void ReadData::setChannelNames(QVector<QString> names){
    channel_names.clear();
    number_data_channels = names.length();
    for (int i=0; i<number_data_channels; i++){
        channel_names[i] = names[i];
    }
}



void ReadData::readSerialData(QString data_string, int event, std::map<QString, double> sensor_zeros){
    QStringList values = data_string.split(",");

    double value;
    QString variable;
    for (int i=0; i<number_data_channels; i++){
        variable = channel_names[i];
        value = values.value(i).toDouble();
        value -= sensor_zeros[variable];
        current_dataset[variable] = value;
    }
}

std::map<int, QString> ReadData::getChannelNames(){
    return channel_names;
}

// SLOTS

std::map<QString, double> ReadData::readCurrentDataset(QString data_string, int event, std::map<QString, double> sensor_zeros){
    readSerialData(data_string,event, sensor_zeros);
    return current_dataset;
}

