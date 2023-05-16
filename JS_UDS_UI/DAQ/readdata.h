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
    std::map<QString, int> getChannelPlotNumbers();
    QVector<QString> getEventCodes();
    QVector<QString> getEventLabels();
public slots:
    void setZeroPressure();

private:
    void setChannelNamesRangesEvents(QVector<QString> names, QVector<QVector<double>> ranges, QVector<QString> event_codes, QVector<QString> event_labels, QVector<int> channel_plot_numbers);
    void readSerialData(QString data_string, int event, bool zero_sensors);

    QString test_type;
    int number_data_channels;
    std::map<int, QString> channel_names;
    QVector<QVector<double>> channel_ranges;
    std::map<QString, double> current_dataset;
    std::map<QString, double> channel_zeros;
    std::map<QString, int> channel_plot_numbers;
    QVector<QString> event_codes;
    QVector<QString> event_labels;
};

#endif // READDATA_H
