#ifndef TRACESDISPLAY_H
#define TRACESDISPLAY_H

#include <QFrame>

namespace Ui {
class TracesDisplay;
}

class TracesDisplay : public QFrame
{
    Q_OBJECT

public:
    explicit TracesDisplay(QWidget *parent = nullptr);
    ~TracesDisplay();

    void loadCurrentDataset(std::map<QString, double> dataset);

private:
    Ui::TracesDisplay *ui;

    void initGraph();
    void initDataset(QVector<QString> channel_names,int no_samples);

    int no_samples;
    QVector<QString> channel_names;
    QVector<QVector<double>> dataset;
};

#endif // TRACESDISPLAY_H
