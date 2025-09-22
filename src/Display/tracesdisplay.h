#ifndef TRACESDISPLAY_H
#define TRACESDISPLAY_H

#include <QFrame>
#include <QVector>
#include "libs/qcustomplot.h"

namespace Ui {
class TracesDisplay;
}

class TracesDisplay : public QFrame {
    Q_OBJECT

  public:
    explicit TracesDisplay(QWidget* parent = nullptr);
    ~TracesDisplay();

    void setHorizontalAxesColour(QPen axes_colour);
    void setDataLineColours(QVector<QPen> line_colours);
    void setBackgroundColour(int red, int green, int blue);

    void setSampleWindowLength(int sample_window_length);
    void setTestingType(QString test);
    void setExitTestingType();

    // void setChannelNames(std::map<int, QString> var_names, QVector<QVector<double>> var_ranges, int sample_window_length);
    void addDataset(std::map<QString, double> curr_dataset);

    // void createEventLine(int event);
    void createEventLine(QString event);
    void updateEventLines();
    void loadEventData(QVector<double> events);

  private:
    Ui::TracesDisplay* ui;

    void setAxesColours();
    double getVarYOffset(int var_number);
    double getVarYScaling(double var_y_span, double var_min, double var_max);
    void setGraphLims();
    void initGraphDatasets();
    void setGraphFrame();
    void setHorizontalAxisLines();
    void setChannelTraces();
    void setYTicks();
    void loadGraphData();
    void clearEventLines();

    std::map<QString, double> convertRawDataset(std::map<QString, double> dataset);

    std::map<int, QString> events_key;

    QVector<QString> variables;
    QVector<QVector<double>> var_ranges;
    QVector<int> plot_numbers;
    std::map<QString, double> y_offsets;
    std::map<QString, double> y_scales;

    QVector<double> time_dataset;
    QVector<QVector<double>> dataset;

    int no_channels;
    int no_samples;
    // QVector<QString> y_channel_names;
    int no_plots;
    double min_y;
    double max_y;
    double y_span;
    double y_buffer;

    QCPAxisRect* graph_rect;
    QVector<QCPItemStraightLine*> horizontal_axes_lines;
    QVector<QCPGraph*> channel_plots;

    int bg_r;
    int bg_g;
    int bg_b;
    QVector<QPen> data_line_colours;
    QPen x_axes_pen;

    QVector<double> y_tick_vals;
    QVector<QString> y_ticks_labs;

    QVector<QCPItemStraightLine*> event_lines;
    QVector<QCPItemText*> event_labels;
};

#endif // TRACESDISPLAY_H
