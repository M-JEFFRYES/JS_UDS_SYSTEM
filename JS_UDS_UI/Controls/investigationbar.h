#ifndef INVESTIGATIONBAR_H
#define INVESTIGATIONBAR_H

#include <QFrame>

namespace Ui {
class InvestigationBar;
}

class InvestigationBar : public QFrame
{
    Q_OBJECT

public:
    explicit InvestigationBar(QWidget *parent = nullptr);
    ~InvestigationBar();
    void setView(QVector<QString> event_codes);
    void resetView();
    void setUDSReadyView(bool set);

public slots:
    void setUDSView();
    void setTestView();

signals:
    void sendOpenNewPatient();
    void sendOpenExistingPatient();
    void sendClosePatient();
    void sendEvent(int);
    void sendStartRecording();
    void sendStopRecording();
    void sendZeroPressure();

private slots:
    void openNewPatient();
    void openExistingPatient();
    void recordInvestigationControl();
    void setZeroPressure();
    void logEvent1();
    void logEvent2();
    void logEvent3();


private:
    Ui::InvestigationBar *ui;

    void initBar();

};

#endif // INVESTIGATIONBAR_H
