#ifndef WRITEDATA_H
#define WRITEDATA_H

#include <QObject>
#include <iostream>
#include <fstream>

class WriteData
{
public:
    WriteData();
    void setFileDirectory(QString dir_path);
    void loadMetaData(std::map<QString, QString> meta_data);
    void setVariableTitles(std::map<int, QString> variables_list);

    void writeDataToCSV(std::map<QString, double> data);
    void createCSVFile();
    bool checkCSVCreated();
    void setEndCSVRecording();

private:
    void createFileName();
    void createMetaDataLines();
    QString getDataLine(std::map<QString, double> data);

    QString directory_path;
    QString file_name;
    QString file_path;
    std::ofstream csv_file;
    bool csv_created;

    std::map<QString, QString> meta_data;
    std::map<int, QString> variable_names;

    QStringList csv_meta_lines;
    QString csv_headers_line;
};

#endif // WRITEDATA_H
