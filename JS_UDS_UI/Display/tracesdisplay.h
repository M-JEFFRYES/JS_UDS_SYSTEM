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

private:
    Ui::TracesDisplay *ui;
};

#endif // TRACESDISPLAY_H
