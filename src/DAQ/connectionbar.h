#ifndef CONNECTIONBAR_H
#define CONNECTIONBAR_H

#include <QFrame>
#include <QPushButton>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class ConnectionBar;
}

class ConnectionBar : public QFrame {
    Q_OBJECT

  public:
    explicit ConnectionBar(QWidget* parent = nullptr);
    ~ConnectionBar();

    void setBarHeights(int minimum, int maximum);

    void setDisconnected();
    bool isPortOpen();
    QPushButton* getConnectionButton();
    QPushButton* getRefreshButton();
    QString getSelectedPort();

  public slots:
    void receiveTestSelected(bool selected);
    void setEnterRecordingMode(bool start);

  signals:
    void sendSerialConnectionMade(bool);

  private slots:
    void on_connectButton_clicked();
    void on_refreshButton_clicked();

  private:
    Ui::ConnectionBar* ui;
    void initButtons();
    void setConnected();

    void setTestSelected(bool selected);
    void getOpenPorts();
};

#endif // CONNECTIONBAR_H
