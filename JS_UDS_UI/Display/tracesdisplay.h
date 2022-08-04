#ifndef TRACESDISPLAY_H
#define TRACESDISPLAY_H

#include <QFrame>
#include <QVector>

namespace Ui {
class TracesDisplay;
}

class TracesDisplay : public QFrame
{
    Q_OBJECT

public:
    explicit TracesDisplay(QWidget *parent = nullptr);
    ~TracesDisplay();

    void setChannelNames(std::map<int, QString> var_names, QVector<QVector<double>> var_ranges);
    void addDataset(std::map<QString, double> dataset);


    void loadCurrentDataset(std::map<QString, double> dataset);

private:
    Ui::TracesDisplay *ui;

    void calculateOffetY();
    void calculateScalingY(QVector<QVector<double>> var_ranges);
    void initDataset();

    std::map<QString, double> convertRawDataset(std::map<QString, double> dataset);

    void initGraph();


    int no_channels;
    int no_samples;
    QVector<QString> y_channel_names;
    std::map<QString, double> y_offsets;
    std::map<QString, double> y_scales;

    QVector<double> time_dataset;
    QVector<QVector<double>> dataset;
};

#endif // TRACESDISPLAY_H
