#include "tracesdisplay.h"
#include "ui_tracesdisplay.h"

#include "AppConstants.h"

TracesDisplay::TracesDisplay(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TracesDisplay)
{
    ui->setupUi(this);
    y_span = 100;
    y_buffer = 25;
}

TracesDisplay::~TracesDisplay()
{
    delete ui;
}

// UI
void TracesDisplay::setBackgroundColour(int red, int green, int blue){
    bg_r = red;
    bg_g = green;
    bg_b = blue;
    ui->graph->setBackground(QBrush(QColor(bg_r, bg_g, bg_b)));
}

void TracesDisplay::setHorizontalAxesColour(QPen axes_colour){
    x_axes_pen = axes_colour;
    setAxesColours();
}

void TracesDisplay::setAxesColours(){
    ui->graph->xAxis->setTickLabelColor(x_axes_pen.color());
    ui->graph->yAxis->setTickLabelColor(x_axes_pen.color());
    ui->graph->xAxis->setBasePen(x_axes_pen);
    ui->graph->yAxis->setBasePen(x_axes_pen);
    ui->graph->xAxis->setTickPen(x_axes_pen);
    ui->graph->yAxis->setTickPen(x_axes_pen);
    ui->graph->xAxis->setSubTickPen(x_axes_pen);
    ui->graph->yAxis->setSubTickPen(x_axes_pen);

}

void TracesDisplay::setDataLineColours(QVector<QPen> line_colours){data_line_colours = line_colours;}

// INIT

void TracesDisplay::setSampleWindowLength(int sample_window_length){no_samples = sample_window_length;}

void TracesDisplay::setTestingType(QString test){
    // variables
    if (test == TestTypeConstants::PRESSURE_TEST_DESC){
        var_ranges = PressureConsts::Y_RANGES;
        variables = PressureConsts::VARS_PLOT;
        plot_numbers = PressureConsts::PLOT_NOS;
    }  else if (test == TestTypeConstants::VOLUME_VOID_TEST_DESC){
        var_ranges = VolumeVoidConsts::Y_RANGES;
        variables = VolumeVoidConsts::VARS_PLOT;
        plot_numbers = VolumeVoidConsts::PLOT_NOS;
    }  else if (test == TestTypeConstants::VOLUME_INFUSED_TEST_DESC){
        var_ranges = VolumeInfusedConsts::Y_RANGES;
        variables = VolumeInfusedConsts::VARS_PLOT;
        plot_numbers = VolumeInfusedConsts::PLOT_NOS;
    }  else if (test == TestTypeConstants::INFUSION_RATE_TEST_DESC){
        var_ranges = InfusionRateConsts::Y_RANGES;
        variables = InfusionRateConsts::VARS_PLOT;
        plot_numbers = InfusionRateConsts::PLOT_NOS;
    }  else if (test == TestTypeConstants::UDS_INVESTIGATION_DESC){
        var_ranges = UDSConsts::Y_RANGES;
        variables = UDSConsts::VARS_PLOT;
        plot_numbers = UDSConsts::PLOT_NOS;
    }
    no_channels = variables.length();

    // events
    events_key = EventConstants::EVENTS;

    //
    y_offsets.clear();
    y_scales.clear();
    clearEventLines();

    int max = 0;
    for (int i=0; i<plot_numbers.length(); i++){
        if(plot_numbers[i] > max){max=plot_numbers[i];}
    }
    no_plots = max +1;

    //
    for (int i=0; i<no_channels; i++){
        y_offsets[variables[i]]= getVarYOffset(i);
        y_scales[variables[i]] = getVarYScaling(y_span, var_ranges[i][0], var_ranges[i][1]);
    }
    setGraphLims();
    initGraphDatasets();
    setGraphFrame();
    setHorizontalAxisLines();
    setChannelTraces();
    setYTicks();

    graph_rect->axis(QCPAxis::atLeft)->setRange(min_y, max_y);
    ui->graph->replot();

}

void TracesDisplay::setExitTestingType(){
    // add func
}

double TracesDisplay::getVarYOffset(int var_number){
    int plot_no = plot_numbers[var_number];
    return (no_plots - plot_no -1) * (y_span + y_buffer);
}

double TracesDisplay::getVarYScaling(double var_y_span, double var_min, double var_max){
    return (var_y_span / (var_max - var_min));
}

void TracesDisplay::setGraphLims(){
    min_y = 0 - y_buffer;
    max_y = (no_plots * (y_span+y_buffer));
}

void TracesDisplay::initGraphDatasets(){
    time_dataset.clear();
    time_dataset.reserve(no_samples);
    for(int i=0; i<no_samples; i++){
        time_dataset.append((double) i); //0.0);
    }

    dataset.clear();
    dataset.reserve(no_channels);
    for (int var=0; var < no_channels; var++){

        double base_val;
        base_val = y_offsets[variables[var]];

        QVector<double> arr;
        arr.reserve(no_samples);
        for(int i=0; i<no_samples; i++){
            arr.append(base_val);
        }
        dataset.insert(var, arr);
    }
}

void TracesDisplay::setGraphFrame(){
    ui->graph->clearGraphs();
    for (int i=0; i<ui->graph->graphCount(); i++){
        ui->graph->graph(0)->data()->clear();
    }
    ui->graph->plotLayout()->clear();
    ui->graph->clearItems();

    // create and set the layout
    QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    ui->graph->plotLayout()->addElement(0,0,subLayout);
    graph_rect = new QCPAxisRect(ui->graph, false);
    subLayout->addElement(0,0, graph_rect);
    graph_rect->addAxes(QCPAxis::atBottom | QCPAxis::atLeft);
}

void TracesDisplay::setHorizontalAxisLines(){
    double offset;
    QString name;

    horizontal_axes_lines.clear();
    setAxesColours();

    for (int i=0; i<no_channels; i++){
        name = variables[i];
        offset = y_offsets[name];

        QCPItemStraightLine *line = new QCPItemStraightLine(ui->graph);
        line->setPen(x_axes_pen);
        line->point1->setCoords(0, offset);  // location of point 1 in plot coordinate
        line->point2->setCoords(no_samples, offset);
        horizontal_axes_lines.append(line);
    }
}

void TracesDisplay::setChannelTraces(){
    channel_plots.clear();

    for (int i=0; i<no_channels; i++){
        QCPGraph* plot = ui->graph->addGraph(graph_rect->axis(QCPAxis::atBottom), graph_rect->axis(QCPAxis::atLeft));
        plot->setPen(data_line_colours[i]);
        plot->addData(time_dataset, dataset.at(i));
        plot->rescaleAxes();
        channel_plots.append(plot);
    }

}

void TracesDisplay::setYTicks(){
    int no_ticks = (no_channels * 3);

    y_tick_vals.clear();
    y_tick_vals.reserve(no_ticks);
    y_ticks_labs.clear();
    y_ticks_labs.reserve(no_ticks);


    double offset;
    double previous_offset = -1;

    for (int i=0; i < no_channels; i++){

        offset = getVarYOffset(i);
        if (previous_offset != offset){
            y_tick_vals.append(offset);
            y_tick_vals.append(offset + y_span);
            y_ticks_labs.append(QString::number(var_ranges.at(i)[0]));
            y_ticks_labs.append(QString::number(var_ranges.at(i)[1]));

        }
        previous_offset = offset;
     }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    ui->graph->yAxis->setTicker(textTicker);
    textTicker->addTicks(y_tick_vals, y_ticks_labs);
    ui->graph->xAxis->setVisible(false);
    ui->graph->xAxis->setLabel(" ");
    ui->graph->yAxis->setLabel(" ");
}

// UPDATING DATASET

void TracesDisplay::addDataset(std::map<QString, double> curr_dataset){
    std::map<QString, double> current_dataset = convertRawDataset(curr_dataset);

    //time_dataset.pop_front();
    //time_dataset.push_back(current_dataset["Time"]);

    QString name;
    double value;
    for (int i=0; i < no_channels; i++){
        name = variables[i];
        value = current_dataset[name];
        dataset[i].pop_front();
        dataset[i].push_back(value);
    }
    loadGraphData();
    ui->graph->replot();
}

std::map<QString, double> TracesDisplay::convertRawDataset(std::map<QString, double> dataset){

    QString name;
    std::map<QString, double> current_dataset;
    for (int i=0; i<no_channels; i++){
        name = variables[i];
        current_dataset[name] = y_offsets[name] + (dataset[name] * y_scales[name]);
    }
    return current_dataset;
}

void TracesDisplay::loadGraphData(){
    for (int i=0; i < no_channels; i++){
        channel_plots[i]->setData(time_dataset, dataset.at(i));
    }
}

// EVENT LINES

void TracesDisplay::createEventLine(QString event){
    QCPItemStraightLine *event_line = new QCPItemStraightLine(ui->graph);
    event_line->setPen(x_axes_pen);
    event_line->point1->setCoords(no_samples-1, min_y);  // location of point 1 in plot coordinate
    event_line->point2->setCoords(no_samples-1, max_y);
    event_lines.append(event_line);

    QCPItemText *event_label = new QCPItemText(ui->graph);
    event_label->setText(event);
    event_label->setPen(x_axes_pen);
    event_label->setColor(x_axes_pen.color());
    event_label->position->setCoords(no_samples-2 , (max_y-y_buffer));
    event_labels.append(event_label);
}

void TracesDisplay::updateEventLines(){
    int x;
    if (event_lines.length() != 0){
        for (int i=0; i<event_lines.length(); i++){
            QCPItemStraightLine *event_line = event_lines.at(i);
            QPointF previous_coord = event_line->point1->coords();

            x = (previous_coord.x()-1);

            if (x == 0){
                event_line->~QCPItemStraightLine();
                event_lines.remove(i);
            } else {
                event_line->point1->setCoords(x, min_y);  // location of point 1 in plot coordinate
                event_line->point2->setCoords(x, max_y);
            }
        }
    }
    if (event_labels.length() != 0){
        for (int i=0; i<event_labels.length(); i++){
            QCPItemText *event_label = event_labels.at(i);
            QPointF previous_coord = event_label->position->coords();
            x = previous_coord.x() - 1;

            if (x == 0){
                event_label->~QCPItemText();
                event_labels.remove(i);
            } else {
                event_label->position->setCoords(x, (max_y-y_buffer));
            }
        }
    }
}

void TracesDisplay::clearEventLines(){
    if (event_lines.length() != 0){
        for (int i=0; i<event_lines.length(); i++){
            event_lines.at(i)->~QCPItemStraightLine();
        }
    }
    event_lines.clear();

    if (event_labels.length() != 0){
        for (int i=0; i<event_labels.length(); i++){
            event_labels.at(i)->~QCPItemText();
        }
    }
    event_labels.clear();
}
