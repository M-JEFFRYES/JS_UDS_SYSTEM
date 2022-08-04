#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void serialReceived();
    void receiveTestType(QString);
    void receiveExitTestType();

private:
    Ui::MainWindow *ui;
    void initConnections();
    void setSizes();

    void setSerialConnection();
    void connectToSerialPort();
    void processIncomingData(QString data_string, int event, bool zero_sensors);


    QString test;
    ReadData data_reader;
    int event_code;

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
};
#endif // MAINWINDOW_H
