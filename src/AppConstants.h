#ifndef EVENTCONSTS_H
#define EVENTCONSTS_H

#include <QObject>

const QString TIME_LAB = "Time";
const int SAMPLE_WINDOW_LENGTH = 200;

namespace DimensionConstants {
const int MAX_CONTROL_BUTTON_WIDTH = 300;
}

namespace SerConnConstants {
const int BAUD_RATE = 9600;
const int START_DATA_TRANSMISSION = 333;
const int END_DATA_TRANSMISSION = 444;
} // namespace SerConnConstants

namespace TestTypeConstants {
const int UDS_INVESTIGATION = 0;
const int PRESSURE_TEST = 1;
const int VOLUME_VOID_TEST = 2;
const int VOLUME_INFUSED_TEST = 3;
const int INFUSION_RATE_TEST = 4;

const QString UDS_INVESTIGATION_DESC = "UDS Clinical Investigation";
const QString PRESSURE_TEST_DESC = "Test - Pressure";
const QString VOLUME_VOID_TEST_DESC = "Test - Volume Void";
const QString VOLUME_INFUSED_TEST_DESC = "Test - Volume Infused";
const QString INFUSION_RATE_TEST_DESC = "Test - Infusion Rate";
} // namespace TestTypeConstants

namespace UDSConsts {
const QVector<QString> VARS_ALL = {"Time", "PBLAD", "PABD", "PDET", "VI", "VV", "Q", "EVENT", "INFRATE"};
const QVector<QString> VARS_PLOT = {"PBLAD", "PABD", "PDET", "VI", "VV", "Q"};
const QVector<QString> VARS_SERCOMM = {"Time", "PBLAD", "PABD", "VI", "VV", "EVENT"};

const QVector<QString> VARS = {"Time", "PBLAD", "PABD", "PDET", "VI", "VV", "Q"};
const QVector<int> PLOT_NOS = {0, 1, 2, 3, 3, 4};
const QVector<QVector<double>> Y_RANGES = {{0.0, 100.0}, {0.0, 100.0}, {0.0, 100.0}, {0.0, 500.0}, {0.0, 500.0}, {0.0, 50.0}};
const QVector<QString> event_codes = {"DO", "Leak", "Valsalva"};
const QVector<QString> event_labels = {"do", "lk", "ch"};
} // namespace UDSConsts

namespace PressureConsts {
const QVector<QString> VARS_ALL = {"Time", "PSENS", "EVENT"};
const QVector<QString> VARS_PLOT = {"PSENS"};
const QVector<QString> VARS_SERCOMM = {"Time", "PSENS", "EVENT"};

const QVector<QString> VARS = {"Time", "PSENS"};
const QVector<int> PLOT_NOS = {0};
const QVector<QVector<double>> Y_RANGES = {{0.0, 100.0}};
const QVector<QString> event_codes = {"First Height", "Second", "Third"};
const QVector<QString> event_labels = {"H1", "H2", "H3"};
} // namespace PressureConsts

namespace VolumeVoidConsts {
const QVector<QString> VARS_ALL = {"Time", "VV", "EVENT"};
const QVector<QString> VARS_PLOT = {"VV"};
const QVector<QString> VARS_SERCOMM = {"Time", "VV", "EVENT"};

const QVector<QString> VARS = {"Time", "UF"};
const QVector<int> PLOT_NOS = {0};
const QVector<QVector<double>> Y_RANGES = {{0.0, 500.0}};
const QVector<QString> event_codes = {"First Height", "Second", "Third"};
const QVector<QString> event_labels = {"H1", "H2", "H3"};
} // namespace VolumeVoidConsts

namespace VolumeInfusedConsts {
const QVector<QString> VARS_ALL = {"Time", "VI", "EVENT"};
const QVector<QString> VARS_PLOT = {"VI"};
const QVector<QString> VARS_SERCOMM = {"Time", "VI", "EVENT"};

const QVector<QString> VARS = {"Time", "VI"};
const QVector<int> PLOT_NOS = {0};
const QVector<QVector<double>> Y_RANGES = {{0.0, 500.0}};
const QVector<QString> event_codes = {"First Height", "Second", "Third"};
const QVector<QString> event_labels = {"H1", "H2", "H3"};
} // namespace VolumeInfusedConsts

namespace InfusionRateConsts {
const QVector<QString> VARS_ALL = {"Time", "PWM", "VI", "EVENT"};
const QVector<QString> VARS_PLOT = {
    "PWM",
    "VI",
};
const QVector<QString> VARS_SERCOMM = {"Time", "PWM", "VI", "EVENT"};

const QVector<QString> VARS = {"Time", "PWM", "UF"};
const QVector<int> PLOT_NOS = {0, 1};
const QVector<QVector<double>> Y_RANGES = {{0.0, 255.0}, {0.0, 500.0}};
const QVector<QString> event_codes = {"First Height", "Second", "Third"};
const QVector<QString> event_labels = {"H1", "H2", "H3"};
} // namespace InfusionRateConsts

namespace EventConstants {
const int NO_EVENT = 0;
const int FIRST_DESIRE = 1;
const int NORMAL_DESIRE = 2;
const int STRONG_DESIRE = 3;
const int URGENCY = 4;
const int DETRUSOR_OVERACTIVITY = 5;
const int LEAK_POINT = 6;
const int START_OF_VOIDING = 7;

const QString NO_EVENT_LAB = "";
const QString FIRST_DESIRE_LAB = "FD";
const QString NORMAL_DESIRE_LAB = "ND";
const QString STRONG_DESIRE_LAB = "SD";
const QString URGENCY_LAB = "UR";
const QString DETRUSOR_OVERACTIVITY_LAB = "DO";
const QString LEAK_POINT_LAB = "LP";
const QString START_OF_VOIDING_LAB = "Void";

const QString NO_EVENT_DESC = "No Event";
const QString FIRST_DESIRE_DESC = "First Desire";
const QString NORMAL_DESIRE_DESC = "Normal Desire";
const QString STRONG_DESIRE_DESC = "Strong Desire";
const QString URGENCY_DESC = "Urgency";
const QString DETRUSOR_OVERACTIVITY_DESC = "Detrusor Overactivity";
const QString LEAK_POINT_DESC = "Leak Point";
const QString START_OF_VOIDING_DESC = "Start of Voiding";

const std::map<int, QString> EVENTS = {
    {NO_EVENT, NO_EVENT_LAB},
    {FIRST_DESIRE, FIRST_DESIRE_LAB},
    {NORMAL_DESIRE, NORMAL_DESIRE_LAB},
    {STRONG_DESIRE, STRONG_DESIRE_LAB},
    {URGENCY, URGENCY_LAB},
    {DETRUSOR_OVERACTIVITY, DETRUSOR_OVERACTIVITY_LAB},
    {LEAK_POINT, LEAK_POINT_LAB},
    {START_OF_VOIDING, START_OF_VOIDING_LAB},
};
} // namespace EventConstants

namespace PumpConstants {
const int PUMP_INPUT_MIN = 0;
const int PUMP_INPUT_MAX = 255;
} // namespace PumpConstants

#endif // EVENTCONSTS_H
