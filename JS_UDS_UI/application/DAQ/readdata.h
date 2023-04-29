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
    QVector<QVector<double>> getChannelRanges();

    std::map<QString, double> readCurrentDataset(QString data_string, int event, bool zero_channels);
    std::map<QString, double> getChannelZeros();

private:
    void setChannelNames(QVector<QString> names);
    void setChannelRanges(QVector<QVector<double>> ranges);
    void readSerialData(QString data_string, int event, bool zero_sensors);


    int number_data_channels;
    std::map<int, QString> channel_names;
    QVector<QVector<double>> channel_ranges;
    std::map<QString, double> current_dataset;
    std::map<QString, double> channel_zeros;
};

#endif // READDATA_H