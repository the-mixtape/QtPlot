#include "MainWindow.h"

#include "Plot/QtPlot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    initializePlot();
    initializeWaterfall();
}

MainWindow::~MainWindow()
{}

void MainWindow::initializePlot()
{
    ui.plot->setMouseTracking(true);
    ui.plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    ui.plot->yAxis->setLabel("y");
    ui.plot->yAxis->setTickLabelColor(Qt::white);
    ui.plot->yAxis->setLabelColor(Qt::white);
    ui.plot->yAxis->setBasePen(QPen(Qt::white));

    ui.plot->xAxis->setLabel("x");
    ui.plot->xAxis->setTickLabelColor(Qt::white);
    ui.plot->xAxis->setLabelColor(Qt::white);
    ui.plot->xAxis->setBasePen(QPen(Qt::white));

	ui.plot->setBackground(Qt::black);
    QCPTextElement* chartSpectrRuntimeText = new QCPTextElement(ui.plot);
    chartSpectrRuntimeText->setText("Marker Test");
    chartSpectrRuntimeText->setFont(QFont("sans", 11, QFont::Bold));
    chartSpectrRuntimeText->setTextColor(Qt::white);

    ui.plot->plotLayout()->insertRow(0);
    ui.plot->plotLayout()->addElement(0, 0, chartSpectrRuntimeText);


    QCPGraph* spectrGraphRuntime = ui.plot->addGraph();
    spectrGraphRuntime->setPen(QPen(QColor(0, 255, 0), 3));

    //Set graph data
    {
        double y;
        for (int x = 0; x <= 100; x++)
        {
            y = rand() % 100;
            spectrGraphRuntime->addData(x, y);
        }
    }

    // Clamp View Zone
    {
        ui.plot->setAxisClamp(EA_xAxis, true);
        ui.plot->setAxisClamp(EA_yAxis, true);

        ui.plot->setAxisClampRange(EA_xAxis, QCPRange(0, 100));
        ui.plot->setAxisClampRange(EA_yAxis, QCPRange(0, 100));

        ui.plot->xAxis->setRange(0, 100);
        ui.plot->yAxis->setRange(0, 100);
    }

    // Setup Markers
    {
        ui.plot->setMarkerAxisActive(EA_xAxis, true);
        ui.plot->setMarkerMovableAxisActive(EA_xAxis, true);

        ui.plot->setMarkerAxisActive(EA_yAxis, true);
        ui.plot->setMarkerMovableAxisActive(EA_yAxis, true);

        //setup markers style
        {
            QPen pen(Qt::PenStyle::SolidLine);
            pen.setWidth(2);
            pen.setColor(Qt::white);

            QPen hoveredPen(Qt::PenStyle::SolidLine);
            hoveredPen.setWidth(3);
            hoveredPen.setColor(Qt::yellow);

            QPen midPen(Qt::PenStyle::DashLine);
            midPen.setWidth(2);
            midPen.setColor(Qt::white);

            {
                ui.plot->setMarkersPen(EA_xAxis, idle, pen);
                ui.plot->setMarkersPen(EA_xAxis, hovered, hoveredPen);
                ui.plot->setMarkersPen(EA_xAxis, dragging, pen);

                ui.plot->setMidMarkerPen(EA_xAxis, idle, midPen);
                ui.plot->setMidMarkerPen(EA_xAxis, hovered, hoveredPen);
                ui.plot->setMidMarkerPen(EA_xAxis, dragging, pen);
                ui.plot->setTextColor(EA_xAxis, Qt::white);
            }

            {
                ui.plot->setMarkersPen(EA_yAxis, idle, pen);
                ui.plot->setMarkersPen(EA_yAxis, hovered, hoveredPen);
                ui.plot->setMarkersPen(EA_yAxis, dragging, pen);

                ui.plot->setMidMarkerPen(EA_yAxis, idle, midPen);
                ui.plot->setMidMarkerPen(EA_yAxis, hovered, hoveredPen);
                ui.plot->setMidMarkerPen(EA_yAxis, dragging, pen);
                ui.plot->setTextColor(EA_yAxis, Qt::white);
            }
        }
    }
}

void MainWindow::initializeWaterfall()
{
    ui.waterfallPlot->setMouseTracking(true);
    ui.waterfallPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    ui.waterfallPlot->setBackground(Qt::black);
    ui.waterfallPlot->setFillColor(Qt::black);
    ui.waterfallPlot->setAppendHeight(5);

    QCPTextElement* chartSpectrRuntimeText = new QCPTextElement(ui.waterfallPlot);
    chartSpectrRuntimeText->setText("Waterfall");
    chartSpectrRuntimeText->setFont(QFont("sans", 11, QFont::Bold));
    chartSpectrRuntimeText->setTextColor(Qt::white);

    ui.waterfallPlot->yAxis->setTickLabelColor(Qt::white);
    ui.waterfallPlot->yAxis->setBasePen(QPen(Qt::white));
    
    ui.waterfallPlot->xAxis->setTickLabelColor(Qt::white);
    ui.waterfallPlot->xAxis->setBasePen(QPen(Qt::white));

    ui.waterfallPlot->plotLayout()->insertRow(0);
    ui.waterfallPlot->plotLayout()->addElement(0, 0, chartSpectrRuntimeText);

    // Clamp View Zone
    {
        ui.waterfallPlot->setAxisClamp(EA_xAxis, true);
        ui.waterfallPlot->setAxisClamp(EA_yAxis, true);

        ui.waterfallPlot->setAxisClampRange(EA_xAxis, QCPRange(0, 100));
        ui.waterfallPlot->setAxisClampRange(EA_yAxis, QCPRange(0, 100));

        ui.waterfallPlot->xAxis->setRange(0, 100);
        ui.waterfallPlot->yAxis->setRange(0, 100);
    }

    // Setup Markers
    {
        ui.waterfallPlot->setMarkerAxisActive(EA_xAxis, true);
        ui.waterfallPlot->setMarkerMovableAxisActive(EA_xAxis, true);

        ui.waterfallPlot->setMarkerAxisActive(EA_yAxis, true);
        ui.waterfallPlot->setMarkerMovableAxisActive(EA_yAxis, true);

        //setup markers style
        {
            QPen pen(Qt::PenStyle::SolidLine);
            pen.setWidth(2);
            pen.setColor(Qt::white);

            QPen hoveredPen(Qt::PenStyle::SolidLine);
            hoveredPen.setWidth(3);
            hoveredPen.setColor(Qt::yellow);

            QPen midPen(Qt::PenStyle::DashLine);
            midPen.setWidth(2);
            midPen.setColor(Qt::white);

            {
                ui.waterfallPlot->setMarkersPen(EA_xAxis, idle, pen);
                ui.waterfallPlot->setMarkersPen(EA_xAxis, hovered, hoveredPen);
                ui.waterfallPlot->setMarkersPen(EA_xAxis, dragging, pen);

                ui.waterfallPlot->setMidMarkerPen(EA_xAxis, idle, midPen);
                ui.waterfallPlot->setMidMarkerPen(EA_xAxis, hovered, hoveredPen);
                ui.waterfallPlot->setMidMarkerPen(EA_xAxis, dragging, pen);
                ui.waterfallPlot->setTextColor(EA_xAxis, Qt::white);
            }

            {
                ui.waterfallPlot->setMarkersPen(EA_yAxis, idle, pen);
                ui.waterfallPlot->setMarkersPen(EA_yAxis, hovered, hoveredPen);
                ui.waterfallPlot->setMarkersPen(EA_yAxis, dragging, pen);

                ui.waterfallPlot->setMidMarkerPen(EA_yAxis, idle, midPen);
                ui.waterfallPlot->setMidMarkerPen(EA_yAxis, hovered, hoveredPen);
                ui.waterfallPlot->setMidMarkerPen(EA_yAxis, dragging, pen);
                ui.waterfallPlot->setTextColor(EA_yAxis, Qt::white);
            }
        }
    }
}
