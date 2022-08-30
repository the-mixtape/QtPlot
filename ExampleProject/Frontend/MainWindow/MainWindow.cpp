#include "MainWindow.h"

#include "Plot/QtPlot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QtPlot* plot = new QtPlot(this);

}

MainWindow::~MainWindow()
{}
