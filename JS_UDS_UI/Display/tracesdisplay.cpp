#include "tracesdisplay.h"
#include "ui_tracesdisplay.h"

TracesDisplay::TracesDisplay(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TracesDisplay)
{
    ui->setupUi(this);
}

TracesDisplay::~TracesDisplay()
{
    delete ui;
}

void TracesDisplay::setChannelNames(std::map<int, QString> var_names, QVector<QVector<double>> var_ranges, int sample_window_length){
    no_samples = sample_window_length;
    no_channels = var_names.size();
    for (int i=1; i < (no_channels); i++){
        y_channel_names.append(var_names[i]);
    }
    y_span = 100;
    max_y = ((y_channel_names.length() * y_span) + 20);

    calculateOffetY();
    calculateScalingY(var_ranges);
    time_dataset.reserve(no_samples);
    dataset.reserve(y_channel_names.length());
    zeroDataset();


    initGraph();
}

void TracesDisplay::calculateOffetY(){
    double y = 0;
    QString var_name;

    var_name = y_channel_names.at(y_channel_names.length()-1);
    y_offsets[var_name] = y;
    for (int i=y_channel_names.length()-2; i>-1; i--){
        var_name = y_channel_names.at(i);
        y += y_span;
        y_offsets[var_name] = y;
    }
}

void TracesDisplay::calculateScalingY(QVector<QVector<double>> var_ranges){

    double actual_range = 100;
    double display_min;
    double display_max;
    double scale;
    QString name;
    QVector<double> vals;

    for (int i=0; i<y_channel_names.length(); i++){
        name = y_channel_names[i];
        vals = var_ranges.at(i);
        display_min = vals.at(0);
        display_max = vals.at(1);
        scale = actual_range/ (display_max - display_min);
        y_scales[name] = scale;
    }
}

void TracesDisplay::zeroDataset(){
    time_dataset.clear();
    for(int i=0; i<no_samples; i++){
        time_dataset.append((double) i); //0.0);
    }

    dataset.clear();
    for (int var=0; var < y_channel_names.length(); var++){
        QVector<double> arr;
        arr.reserve(no_samples);
        double base_val;
        base_val = y_offsets[y_channel_names.at(var)];
        for(int i=0; i<no_samples; i++){
            arr.append(base_val);
        }
        dataset.insert(var, arr);
    }
}


// graph

void TracesDisplay::initGraph(){
    setGraphFrame();
    setHorizontalAxisLines();
    setChannelTraces();

    ui->graph->replot();
}

void TracesDisplay::setGraphFrame(){
    ui->graph->plotLayout()->clear();

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

    for (int i=0; i<y_channel_names.length(); i++){
        name = y_channel_names.at(i);
        offset = y_offsets[name];

        QCPItemStraightLine *line = new QCPItemStraightLine(ui->graph);
        line->setPen(QPen(QColor(0,200,0)));
        line->point1->setCoords(0, offset);  // location of point 1 in plot coordinate
        line->point2->setCoords(no_samples, offset);
        horizontal_axes_lines.append(line);
    }
    graph_rect->axis(QCPAxis::atLeft)->setRange(QCPRange(0, max_y));

}

void TracesDisplay::setChannelTraces(){

    for (int i=0; i<y_channel_names.length(); i++){
        QCPGraph* plot = ui->graph->addGraph(graph_rect->axis(QCPAxis::atBottom), graph_rect->axis(QCPAxis::atLeft));
        plot->addData(time_dataset, dataset.at(i));
        plot->rescaleAxes();
        channel_plots.append(plot);
    }

}





// loading

void TracesDisplay::addDataset(std::map<QString, double> curr_dataset){
    std::map<QString, double> current_dataset = convertRawDataset(curr_dataset);

    //time_dataset.pop_front();
    //time_dataset.push_back(current_dataset["Time"]);

    QString name;
    double value;
    for (int i=0; i < y_channel_names.length(); i++){
        name = y_channel_names.at(i);
        value = (current_dataset[name] * y_scales[name]) + y_offsets[name];
        dataset[i].pop_front();
        dataset[i].push_back(value);
        qDebug() << name << ": " << value;
    }

    loadGraphData();
    ui->graph->replot();
}

std::map<QString, double> TracesDisplay::convertRawDataset(std::map<QString, double> dataset){

    QString name;
    std::map<QString, double> current_dataset;
    for (int i=0; i<y_channel_names.length(); i++){
        name = y_channel_names[i];
        current_dataset[name] = y_offsets[name] + (dataset[name] * y_scales[name]);
    }
    return current_dataset;
}

void TracesDisplay::loadGraphData(){
    for (int i=0; i < y_channel_names.length(); i++){
        channel_plots[i]->setData(time_dataset, dataset.at(i));
    }
}













