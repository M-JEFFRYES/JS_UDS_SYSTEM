#ifndef READDATA_H
#define READDATA_H

#include <QObject>
#include <QVector>


class ReadData
{
public:
    ReadData();
    void setTestingType(const QString& testing_type);


    std::map<QString, double> readCurrentDataset(QString data_string, int event, bool zero_channels);
    std::map<QString, double> getChannelZeros();
    std::map<QString, int> getChannelPlotNumbers();
    QVector<QString> getEventCodes();
    QVector<QString> getEventLabels();
    double getInfusionFlowrate();

public slots:
    void setZeroPressure();

private:
    void initCalculationArrays();
    void updateTimeValues(double time);
    void updateViValues(double vi);
    void updateVvValues(double vv);


    //void setChannelNamesRangesEvents(QVector<QString> names, QVector<QVector<double>> ranges, QVector<QString> event_codes, QVector<QString> event_labels, QVector<int> channel_plot_numbers);
    void readSerialDataset(QString data_string, int event, bool zero_sensors);

    QString test_type;
    QVector<QString> all_variables;
    QVector<QString> ser_comm_variables;
    QVector<QString> plot_variables;

    std::map<QString, double> channel_zeros;

    std::map<QString, double> temp_dataset;



    int number_data_channels;
    QVector<QString> channel_var_names;
    std::map<int, QString> channel_names;
    QVector<QVector<double>> channel_ranges;
    std::map<QString, double> current_dataset;
    std::map<QString, int> channel_plot_numbers;
    QVector<QString> event_codes;
    QVector<QString> event_labels;

    QVector<double> time_values;
    QVector<double> vi_values;
    QVector<double> vv_values;

    double delta_t;
    double delta_vv;
    double delta_vi;
    double q_flowrate;
    double infusion_flowrate;
};

#endif // READDATA_H
