#include "patientinfoentry.h"
#include "ui_patientinfoentry.h"

#include <QDate>
#include <map>

#include <QDebug>

PatientInfoEntry::PatientInfoEntry(QWidget* parent) : QDialog(parent),
                                                      ui(new Ui::PatientInfoEntry) {
    ui->setupUi(this);
    setInvestigationDate();

    connect(ui->okButton, &QPushButton::clicked, this, &PatientInfoEntry::submitPatientInfoEntry);
    connect(ui->cancelButton, &QPushButton::clicked, this, &PatientInfoEntry::cancelPatientInfoEntry);
}

PatientInfoEntry::~PatientInfoEntry() {
    delete ui;
}

void PatientInfoEntry::setInvestigationDate() {
    investigationDate = QDate::currentDate().toString("dd/MM/yyyy");
    ui->dateLabel->setText(investigationDate);
}

void PatientInfoEntry::calculateAge() {
    int days = QDate::currentDate().toJulianDay() - ui->dobInput->date().toJulianDay();
    currentAge = (double)days / 365.25;
}

void PatientInfoEntry::retrievePatientInformation() {
    bool missing_items = false;
    QString msg = "Missing: ";

    calculateAge();

    patient_data["first_name"] = ui->fnameInput->text();
    patient_data["surname"] = ui->snameInput->text();
    patient_data["hospital_number"] = ui->hospitalNumberInput->text();
    patient_data["investigation_date"] = investigationDate;
    patient_data["investigation_type"] = "UDS";
    patient_data["dob"] = ui->dobInput->date().toString("dd/MM/yyyy");
    patient_data["age"] = QString::number(currentAge);

    if (patient_data["first_name"].length() == 0) {
        missing_items = true;
        msg = msg + "First Name, ";
    }
    if (patient_data["surname"].length() == 0) {
        missing_items = true;
        msg = msg + "Surame, ";
    }
    if (patient_data["hospital_number"].length() == 0) {
        missing_items = true;
        msg = msg + "Hospital No, ";
    }

    if (missing_items) {
        ui->missingLabel->setText(msg);
    }

    patientInfoComplete = !missing_items;
}

void PatientInfoEntry::submitPatientInfoEntry() {
    retrievePatientInformation();
    if (patientInfoComplete) {
        done(QDialog::Accepted);
    }
}

void PatientInfoEntry::cancelPatientInfoEntry() {
    done(QDialog::Rejected);
}

std::map<QString, QString> PatientInfoEntry::getPatientInformation() {
    return patient_data;
}

QString PatientInfoEntry::getWindowTitle() {
    QString title = "UDS Investigaion: ";
    title += investigationDate;
    title += " - ";
    title += patient_data["first_name"];
    title += " ";
    title += patient_data["surname"];
    title += " (";
    title += patient_data["hospital_number"];
    title += ", ";
    title += patient_data["dob"];
    title += ")";

    return title;
}
