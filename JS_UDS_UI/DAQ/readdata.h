#ifndef READDATA_H
#define READDATA_H

#include <QObject>

class ReadData
{
public:
    ReadData();
    void setTestingType(QString testing_type);

    std::map<QString, double> readCurrentDataset(QString data_string, int event, std::map<QString, double> sensor_zeros);


private:
    void setPressureMeasurementTest();
    void setUroflowmetryTest();
    void setVolumeInfusedTest();
    void setPumpTest();

    void readSerialData(QString data_string, int event, std::map<QString, double> sensor_zeros);


    int number_data_channels;
    QVector<QString> channel_names;
    std::map<QString, double> current_dataset;
};

#endif // READDATA_H
