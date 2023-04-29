#include "writedata.h"

#include <QDir>
#include <QObject>
#include <QDebug>
#include <QDate>
#include <QTime>

WriteData::WriteData()
{
    csv_created = false;
}

// Pre recording settings

void WriteData::setFileDirectory(QString dir_path){directory_path = dir_path;}

void WriteData::setVariableTitles(std::map<int, QString> variable_names){
    this->variable_names = variable_names;

    int no_channels = variable_names.size();
    csv_headers_line = variable_names[0];
    for (int i=1; i< no_channels; i++){
        csv_headers_line += ",";
        csv_headers_line += variable_names[i];
    }
    csv_headers_line += "\n";
}

void WriteData::loadMetaData(std::map<QString, QString> meta_data){
    this->meta_data = meta_data;
}

// Pre recording formatting

void WriteData::createFileName(){
    file_name = "";

    QString date = QDate::currentDate().toString("dd/MM/yyyy");
    QStringList date_list = date.split("/");
    for (int i=2; i>=0; i--){
        file_name += date_list[i];
    }
    file_name += "_";
    QString time = QTime::currentTime().toString("HH:mm:ss");
    QStringList time_list = time.split(":");
    for (int i=0; i<3; i++){
        file_name += time_list[i];
    }

    file_name += "_";
    file_name += meta_data["hospital_number"];
    file_name += "_";
    file_name += meta_data["investigation_type"];
    file_name += ".csv";

    file_path = QDir(directory_path).filePath(file_name);
}

void WriteData::createMetaDataLines(){

    QVector<QString> vars = {
        "investigation_date", "investigation_type", "first_name", "surname",
        "hospital_number","dob", "age"
    };

    for (int i=0; i<vars.length(); i++){
        QString line = "#,";
        line += vars[i];
        line += ",";
        line += meta_data[vars[i]];
        line += "\n";
        csv_meta_lines.append(line);
    }
}


// recording functions
void WriteData::createCSVFile(){
    createFileName();
    createMetaDataLines();
    csv_file.open(file_path.toStdString());

    for (int i=0; i<csv_meta_lines.length(); i++){
        csv_file << csv_meta_lines[i].toStdString();
    }

    csv_file << "\n";
    csv_file << csv_headers_line.toStdString();
    csv_file.close();
    csv_created = true;
    qInfo() << "CSV file created --> " << file_path;
}

bool WriteData::checkCSVCreated(){return csv_created;}

void WriteData::setEndCSVRecording(){
    csv_created = false;
}

QString WriteData::getDataLine(std::map<QString, double> data){
    QString line = QString::number(data[variable_names[0]]);
    int len = variable_names.size();
    for (int i=1; i<len; i++){
        line += ",";
        line += QString::number(data[variable_names[i]]);
    }
    line += "\n";
    return line;
}

void WriteData::writeDataToCSV(std::map<QString, double> data){
    QString line = getDataLine(data);
    csv_file.open(file_path.toStdString(), std::ios::app);
    csv_file << line.toStdString();
    csv_file.close();
}
