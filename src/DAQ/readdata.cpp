#include "readdata.h"
#include "DAQ/testconfigs.h"
#include "AppConstants.h"
#include <QDebug>


ReadData::ReadData()
{
    initCalculationArrays();
}

// CALCULATIONS

void ReadData::initCalculationArrays(){
    time_values = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    vi_values = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    vv_values = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    delta_t = 0;
    delta_vv = 0;
    delta_vi = 0;
    infusion_flowrate = 0;
    q_flowrate = 0;
}

void ReadData::updateTimeValues(double time){
    time_values.pop_back();
    time_values.prepend(time);
    delta_t = time_values.first() - time_values.last();
}

void ReadData::updateViValues(double vi){
    vi_values.pop_back();
    vi_values.prepend(vi);
    delta_vi = vi_values.first() - vi_values.last();
    infusion_flowrate = delta_vi / delta_t;
}

void ReadData::updateVvValues(double vv){
    vv_values.pop_back();
    vv_values.prepend(vv);
    delta_vv = vv_values.first() - vv_values.last();
    q_flowrate = delta_vi / delta_t;
}

// FUNCTIONALITY
void ReadData::setTestingType(const QString& testing_type) {
    if (test_type == testing_type)
        return;

    const auto it = kTestConfigs.constFind(testing_type);
    if (it == kTestConfigs.cend()) {
        qWarning() << "Unknown test type:" << testing_type
                   << "— keeping previous type:" << test_type;
        return; // or choose a default
    }

    test_type = testing_type;
    const TestConfig& cfg = it.value();
    all_variables      = cfg.all;
    ser_comm_variables = cfg.sercomm;
    plot_variables     = cfg.plot;
}

void ReadData::readSerialDataset(QString data_string, int event, bool zero_sensors){

    QStringList valuePairs = data_string.split(";");
    QStringList keyPair;

    double value;
    QString variable;

    for (int i=0; i<valuePairs.length(); i++){
        keyPair = valuePairs[i].split(",");
        variable = keyPair[0];
        value = keyPair[1].toDouble();

        if (variable=="Time"){
            value = value / 1000000;

        } else if (variable == "PSENS" || variable == "PBLAD"|| variable == "PABD") {
            if (zero_sensors){
                channel_zeros[variable] = value;
            }
            value -= channel_zeros[variable];
        }

        current_dataset[variable] = value;
    }

    current_dataset["EVENT"] = event;

    updateTimeValues(current_dataset["Time"]);

    if (test_type == TestTypeConstants::UDS_INVESTIGATION_DESC){
        updateViValues(current_dataset["VI"]);
        updateVvValues(current_dataset["VV"]);

        current_dataset["Q"] = q_flowrate;
        current_dataset["INFRATE"] = infusion_flowrate;
        current_dataset["PDET"] = current_dataset["PBLAD"] - current_dataset["PABD"];

    } else if (test_type == TestTypeConstants::VOLUME_VOID_TEST_DESC){
        updateVvValues(current_dataset["VV"]);
        current_dataset["Q"] = q_flowrate;

    } else if (test_type == TestTypeConstants::VOLUME_INFUSED_TEST_DESC || test_type == TestTypeConstants::INFUSION_RATE_TEST_DESC){
        updateVvValues(current_dataset["VI"]);
        current_dataset["INFRATE"] = infusion_flowrate;
    }
}

void ReadData::setZeroPressure(){
    if (test_type == TestTypeConstants::UDS_INVESTIGATION_DESC){
        channel_zeros["PBLAD"] = current_dataset["PBLAD"];
        channel_zeros["PABD"] = current_dataset["PABD"];

    } else if (test_type == TestTypeConstants::PRESSURE_TEST_DESC) {
        channel_zeros["PSENS"] = current_dataset["PSENS"];
    }
}

// GETTERS

double ReadData::getInfusionFlowrate(){return infusion_flowrate;}

// FUNCTIONALITY
/*
void ReadData::setTestingType(QString testing_type){
    qInfo() << "Setting type" << testing_type;
    test_type = testing_type;
    QVector<QString> names;
    QVector<QVector<double>> ranges;
    QVector<int> plot_no;
    QVector<QString> event_codes;
    QVector<QString> event_labels;

    if (test_type == TestTypeConstants::PRESSURE_TEST_DESC){
        channel_var_names = PressureConsts::VARS;
        channel_ranges = PressureConsts::Y_RANGES;

        names =
        plot_no = PressureConsts::PLOT_NOS;
        ranges =
        event_codes = {"First Height", "Second", "Third"};
        event_labels = {"H1", "H2", "H3"};
        setChannelNamesRangesEvents(
                    PressureConsts::VARS,
                    PressureConsts::Y_RANGES,
                    event
                    event_labels,
                    PressureConsts::PLOT_NOS
                    );

    }  else if (test_type == TestTypeConstants::VOLUME_VOID_TEST_DESC){
        names = VolumeVoidConsts::VARS;
        plot_no = VolumeVoidConsts::PLOT_NOS;
        ranges = VolumeVoidConsts::Y_RANGES;
        event_codes = {"First Height", "Second", "Third"};
        event_labels = {"H1", "H2", "H3"};
        setChannelNamesRangesEvents(names, ranges, event_codes, event_labels, plot_no);

    }  else if (test_type == TestTypeConstants::VOLUME_INFUSED_TEST_DESC){
        names = VolumeInfusedConsts::VARS;
        plot_no = VolumeInfusedConsts::PLOT_NOS;
        ranges = VolumeInfusedConsts::Y_RANGES;
        event_codes = {"First Height", "Second", "Third"};
        event_labels = {"H1", "H2", "H3"};
        setChannelNamesRangesEvents(names, ranges, event_codes, event_labels, plot_no);

    }  else if (test_type == TestTypeConstants::INFUSION_RATE_TEST_DESC){
        names = InfusionRateConsts::VARS;
        plot_no = InfusionRateConsts::PLOT_NOS;
        ranges = InfusionRateConsts::Y_RANGES;
        event_codes = {"First Height", "Second", "Third"};
        event_labels = {"H1", "H2", "H3"};
        setChannelNamesRangesEvents(names, ranges, event_codes, event_labels, plot_no);

    }  else if (test_type == TestTypeConstants::UDS_INVESTIGATION_DESC){
        names = UDSConsts::VARS;
        plot_no = UDSConsts::PLOT_NOS;
        ranges = UDSConsts::Y_RANGES;
        event_codes = {"DO", "Leak", "Valsalva"};
        event_labels = {"do", "lk", "ch"};
        setChannelNamesRangesEvents(names, ranges, event_codes, event_labels, plot_no);
    }
}*/

/*
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
*/

// SLOTS

std::map<QString, double> ReadData::readCurrentDataset(QString data_string, int event, bool zero_channels){
    readSerialDataset(data_string,event, zero_channels);
    return current_dataset;
}

