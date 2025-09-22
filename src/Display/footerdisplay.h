#ifndef FOOTERDISPLAY_H
#define FOOTERDISPLAY_H

#include <QFrame>

namespace Ui {
class FooterDisplay;
}

class FooterDisplay : public QFrame
{
    Q_OBJECT

public:
    explicit FooterDisplay(QWidget *parent = nullptr);
    ~FooterDisplay();

private:
    Ui::FooterDisplay *ui;
};

#endif // FOOTERDISPLAY_H
