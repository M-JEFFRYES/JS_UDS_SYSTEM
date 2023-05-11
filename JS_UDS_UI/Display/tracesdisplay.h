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

    void setHorizontalAxesColour(QPen axes_colour);
    void setDataLineColours(QVector<QPen> line_colours);
    void setBackgroundColour(int red, int green, int blue);

    void setSampleWindowLength(int sample_window_length);
    void setChannelNamesAndRanges(std::map<int, QString> var_names, QVector<QVector<double>> var_ranges);

    void setChannelNames(std::map<int, QString> var_names, QVector<QVector<double>> var_ranges, int sample_window_length);
    void addDataset(std::map<QString, double> curr_dataset);


private:
    Ui::TracesDisplay *ui;

    double getVarYOffset(int var_number);
    double getVarYScaling(double var_y_span, double var_min, double var_max);
    void setGraphLims();
    void initGraphDatasets();
    //

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
    double min_y;
    double max_y;
    double y_span;
    double y_buffer;

    QVector<double> time_dataset;
    QVector<QVector<double>> dataset;

    QCPAxisRect* graph_rect;
    QVector<QCPItemStraightLine*> horizontal_axes_lines;
    QVector<QCPGraph*> channel_plots;

    int bg_r;
    int bg_g;
    int bg_b;
    QVector<QPen> data_line_colours;
    QPen x_axes_pen;
};

#endif // TRACESDISPLAY_H
