#ifndef VALUEDISPLAYBAR_H
#define VALUEDISPLAYBAR_H

#include <QFrame>
#include <QLabel>
#include <QLCDNumber>

namespace Ui {
class ValueDisplayBar;
}

class ValueDisplayBar : public QFrame
{
    Q_OBJECT

public:
    explicit ValueDisplayBar(QWidget *parent = nullptr);
    ~ValueDisplayBar();

    void setVarColours(QVector<QPen> var_colours);
    void setDisplayChannels(std::map<int, QString> display_names);
    void displayReset();
    void updateNumbers(std::map<QString, double> data);

private:
    Ui::ValueDisplayBar *ui;

    void initDisplays();

    int no_display_names;
    std::map<int, QString> display_names;
    QVector<QLabel*> display_labels;
    QVector<QLCDNumber*> display_numbers;
    std::map<QString, double> current_dataset;
    std::map<QString, int> variable_mapping;

    QVector<QPen> var_colours;


};

#endif // VALUEDISPLAYBAR_H
