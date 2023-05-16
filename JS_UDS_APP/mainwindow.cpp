#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "../JS_UDS_LOGIC/js_uds_logic.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

