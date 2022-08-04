#ifndef TRACESDISPLAY_H
#define TRACESDISPLAY_H

#include <QFrame>
#include <QVector>
#include "libs/qcustomplot.h"

namespace Ui {
class TracesDisplay;
}

class TracesDisplay : public QFrame
{
    Q_OBJECT

public:
    explicit TracesDisplay(QWidget *parent = nullptr);
    ~TracesDisplay();

    void setChannelNames(std::map<int, QString> var_names, QVector<QVector<double>> var_ranges, int sample_window_length);
    void addDataset(std::map<QString, double> curr_dataset);


private:
    Ui::TracesDisplay *ui;

    void calculateOffetY();
    void calculateScalingY(QVector<QVector<double>> var_ranges);
    void zeroDataset();

    void initGraph();
    void setGraphFrame();
    void setHorizontalAxisLines();
    void setChannelTraces();
    void loadGraphData();

    std::map<QString, double> convertRawDataset(std::map<QString, double> dataset);


    int no_channels;
    int no_samples;
    QVector<QString> y_channel_names;
    std::map<QString, double> y_offsets;
    std::map<QString, double> y_scales;
    double max_y;
    double y_span;

    QVector<double> time_dataset;
    QVector<QVector<double>> dataset;

    QCPAxisRect* graph_rect;
    QVector<QCPItemStraightLine*> horizontal_axes_lines;
    QVector<QCPGraph*> channel_plots;

};

#endif // TRACESDISPLAY_H
