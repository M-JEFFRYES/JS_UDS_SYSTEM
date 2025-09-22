#ifndef PATIENTINFOENTRY_H
#define PATIENTINFOENTRY_H

#include <QDialog>
#include <QDate>
#include <map>

namespace Ui {
class PatientInfoEntry;
}

class PatientInfoEntry : public QDialog {
    Q_OBJECT

  public:
    explicit PatientInfoEntry(QWidget* parent = nullptr);
    ~PatientInfoEntry();

    std::map<QString, QString> getPatientInformation();
    QString getWindowTitle();

  private slots:
    void submitPatientInfoEntry();
    void cancelPatientInfoEntry();

  private:
    Ui::PatientInfoEntry* ui;

    void setInvestigationDate();
    void calculateAge();
    void retrievePatientInformation();

    QString investigationDate;
    double currentAge;
    std::map<QString, QString> patient_data;

    bool patientInfoComplete;
};

#endif // PATIENTINFOENTRY_H
