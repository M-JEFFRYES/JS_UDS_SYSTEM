#ifndef PUMPCONTROL_H
#define PUMPCONTROL_H

#include <QFrame>

namespace Ui {
class PumpControl;
}

class PumpControl : public QFrame
{
    Q_OBJECT

public:
    explicit PumpControl(QWidget *parent = nullptr);
    ~PumpControl();
    void resetPump();
    int getPumpPower();
    int getPumpArduinoInput();

signals:
    void sendPumpRate(int);

private slots:
    void addOneToPump();
    void addTenToPump();
    void minusOneToPump();
    void minusTenToPump();

private:
    Ui::PumpControl *ui;
    void initBar();
    void convertToFlowrate();

    int pump_rate_percentage;
    int pump_rate_arduino_input;

};

#endif // PUMPCONTROL_H
