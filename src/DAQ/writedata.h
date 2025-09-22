#ifndef WRITEDATA_H
#define WRITEDATA_H

#include <QObject>
#include <iostream>
#include <fstream>

class WriteData {
  public:
    WriteData();
    void setFileDirectory(QString dir_path);
    void loadMetaData(std::map<QString, QString> meta_data);

    void setTestingType(QString test);

    void setVariableTitles(QVector<QString> variables_list);

    void writeDataToCSV(std::map<QString, double> data);
    void createCSVFile();
    bool checkCSVCreated();
    void setEndCSVRecording();

  private:
    void createFileName();
    void createMetaDataLines();
    QString getDataLine(std::map<QString, double> data);

    QVector<QString> variables;
    int no_display_names;

    QString directory_path;
    QString file_name;
    QString file_path;
    std::ofstream csv_file;
    bool csv_created;

    std::map<QString, QString> meta_data;
    std::map<QString, int> variable_mapping;
    std::map<int, QString> variable_mapping_inv;

    QStringList csv_meta_lines;
    QString csv_headers_line;
};

#endif // WRITEDATA_H
