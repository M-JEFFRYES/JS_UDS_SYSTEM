#include "footerdisplay.h"
#include "ui_footerdisplay.h"

FooterDisplay::FooterDisplay(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FooterDisplay)
{
    ui->setupUi(this);
}

FooterDisplay::~FooterDisplay()
{
    delete ui;
}
