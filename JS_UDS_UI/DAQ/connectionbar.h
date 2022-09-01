#ifndef CONNECTIONBAR_H
#define CONNECTIONBAR_H

#include <QFrame>
#include <QPushButton>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class ConnectionBar;
}

class ConnectionBar : public QFrame
{
    Q_OBJECT

public:
    explicit ConnectionBar(QWidget *parent = nullptr);
    ~ConnectionBar();

    void setBarHeights(int minimum, int maximum);
    void setEnterRecordingMode(bool start);

    bool isPortOpen();
    QPushButton* getConnectionButton();
    QPushButton* getRefreshButton();
    QString getSelectedPort();


public slots:
    void receiveTestSelected(bool selected);

private slots:
    void on_connectButton_clicked();
    void on_refreshButton_clicked();

private:
    Ui::ConnectionBar *ui;
    void initButtons();
    void setConnected();
    void setDisconnected();
    void setTestSelected(bool selected);
    void getOpenPorts();
};

#endif // CONNECTIONBAR_H
