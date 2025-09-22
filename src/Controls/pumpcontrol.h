#ifndef PUMPCONTROL_H
#define PUMPCONTROL_H

#include <QFrame>

namespace Ui {
class PumpControl;
}

class PumpControl : public QFrame {
    Q_OBJECT

  public:
    explicit PumpControl(QWidget* parent = nullptr);
    ~PumpControl();

    void setTestingType(QString test);
    void setExitTestingType();

    void resetPump();
    int getPumpArduinoInput();
    void setPumpInfusionRate(double infusion_rate);

  public slots:
    void setPumpButtonsEnabled(bool);

  signals:
    void sendPumpRate(int);

  private slots:
    void addOneToPump();
    void addTenToPump();
    void minusOneToPump();
    void minusTenToPump();

  private:
    Ui::PumpControl* ui;
    void initBar();
    void enablePump(bool enable);
    int pump_rate_arduino_input;
};

#endif // PUMPCONTROL_H
