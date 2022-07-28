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

    void setDisplayChannels(QVector<QString> display_names);
    void displayReset();
    void updateNumbers(std::map<QString, double> data);

private:
    Ui::ValueDisplayBar *ui;

    void initDisplays();

    QVector<QString> display_names;
    QVector<QLabel*> display_labels;
    QVector<QLCDNumber*> display_numbers;
    std::map<QString, double> current_dataset;
    std::map<QString, int> variable_mapping;




};

#endif // VALUEDISPLAYBAR_H
