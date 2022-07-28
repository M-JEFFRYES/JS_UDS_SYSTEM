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
