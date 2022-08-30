#include "MainWindow.h"

#include "Plot/QtPlot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    initializePlot();
}

MainWindow::~MainWindow()
{}

void MainWindow::initializePlot()
{
    ui.plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // Clamp View Zone
    {
        ui.plot->setAxisClamp(EA_xAxis, true);
        ui.plot->setAxisClamp(EA_yAxis, true);

        ui.plot->setAxisClampRange(EA_xAxis, QCPRange(0, 100));
        ui.plot->setAxisClampRange(EA_yAxis, QCPRange(0, 100));
    }

    // Enable Markers
    {
        ui.plot->setMarkerAxisActive(EA_xAxis, true);
        ui.plot->setMarkerAxisActive(EA_yAxis, true);
    }
}
