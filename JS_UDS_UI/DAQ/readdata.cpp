#include "ReadData.h"

ReadData::ReadData()
{

}

// FUNCTIONALITY
void ReadData::setTestingType(QString testing_type){
    if (testing_type == "Pressure"){
        setPressureMeasurementTest();
    }  else if (testing_type == "Uroflowmetry"){
        setUroflowmetryTest();
    }  else if (testing_type == "Volume Infused"){
        setVolumeInfusedTest();
    }  else if (testing_type == "Pump"){
        setPumpTest();
    }
}

void ReadData::setPressureMeasurementTest(){
    number_data_channels = 2;
    channel_names.clear();
    channel_names.append("Time");
    channel_names.append("PSENS");
}

void ReadData::setUroflowmetryTest(){
    number_data_channels = 2;
    channel_names.clear();
    channel_names.append("Time");
    channel_names.append("UF_RAW");
}

void ReadData::setVolumeInfusedTest(){
    number_data_channels = 2;
    channel_names.clear();
    channel_names.append("Time");
    channel_names.append("VI_RAW");
}

void ReadData::setPumpTest(){
    number_data_channels = 3;
    channel_names.clear();
    channel_names.append("Time");
    channel_names.append("PWM");
    channel_names.append("UF_RAW");
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

// SLOTS

std::map<QString, double> ReadData::readCurrentDataset(QString data_string, int event, std::map<QString, double> sensor_zeros){
    readSerialData(data_string,event, sensor_zeros);
    return current_dataset;
}

