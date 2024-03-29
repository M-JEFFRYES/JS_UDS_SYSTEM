#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Popups/patientinfoentry.h"
#include "DAQ/writedata.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <DAQ/readdata.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendConnectionOpen(bool);
    void sendSetUDSView();
    void sendSetTestView();
    void sendEnterRecording(bool);
    void sendEnablePumpButtons(bool);

private slots:
    void serialReceived();
    void receiveTestType(QString);
    void receiveExitTestType();
    void receiveOpenNewPatient();
    void receiveOpenExisitngPatient();
    void receiveClosePatient();
    void recieveStartRecordingCSV();
    void recieveStopRecordingCSV();
    //void recieveEventCode(int);
    void recieveEventCode(int);
    void recieveZeroPressure();
    void recievePumpValue(int);

private:
    Ui::MainWindow *ui;
    void initConnections();
    int calculatePixels(double length, double percentage);
    void calculateObjectSizes();
    void setSizes();

    void setSerialConnection();
    void connectToSerialPort();
    void closeSerialConnection();
    void processIncomingData(QString data_string);

    void writeToSerialPort(int value);

    QString generic_window_title;
    std::map<QString, int> pixel_sizes;
    QString test;
    ReadData data_reader;
    WriteData data_writer;
    bool write_to_csv;
    bool csv_created;
    int event_code;
    bool to_zero_sensors;

    // connection
    QSerialPort *conn;
    QString portName;
    int baudRate;
    QString dataBits;
    QString _parity;
    QString stopBits;
    QString _flowControl;

    // data buffer
    bool readSerialData;
    QByteArray serialData;
    QString serialBuffer;

    // patient
    bool patient_entered;

    // data processing
    QVector<QString> variable_names;
};
#endif // MAINWINDOW_H
