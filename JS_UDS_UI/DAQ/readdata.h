#ifndef READDATA_H
#define READDATA_H

#include <QObject>
#include <QVector>


class ReadData
{
public:
    ReadData();
    void setTestingType(QString testing_type);

    std::map<int, QString> getChannelNames();
    std::map<QString, double> readCurrentDataset(QString data_string, int event, std::map<QString, double> sensor_zeros);


private:
    void setChannelNames(QVector<QString> names);
    void readSerialData(QString data_string, int event, std::map<QString, double> sensor_zeros);


    int number_data_channels;
    std::map<int, QString> channel_names;
    std::map<QString, double> current_dataset;
};

#endif // READDATA_H
