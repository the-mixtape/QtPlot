#include "MainWindow.h"

#include "Plot/QtPlot.h"
#include "Plot/Items/SingleMarker.h"
#include "Waterfall/WaterfallContent.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    initializePlot();
    initializeWaterfall();
    initializeSyncPlots();

    backend.start();

    connect(&backend, &ExampleBackend::generatedNewData, [=](double* data, int size) { ui.waterfallPlot->appendData(data, size); });
    connect(ui.waterfallPlot, &WaterfallPlot::copyingCompleted, &backend, &ExampleBackend::copyingCompleted);
}

MainWindow::~MainWindow()
{
}

void MainWindow::initializePlot()
{
    initializePlot(ui.plot, "Marker Test");

    ui.plot->enableTooltip();
    ui.plot->setShowInfoRule(ESIR_Attach);

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
}

void MainWindow::initializeWaterfall()
{
    initializePlot(ui.waterfallPlot, "Waterfall");

    ui.waterfallPlot->setBackground(Qt::black);
    ui.waterfallPlot->setFillColor(Qt::black);
    ui.waterfallPlot->setAppendHeight(1);
    ui.waterfallPlot->setAppendSide(EAS_Right);
    ui.waterfallPlot->setResolution(10000, 512);
    ui.waterfallPlot->setFPSLimit(30);
}

void MainWindow::initializeSyncPlots()
{
    initializePlot(ui.syncPlot1, "Sync Plot 1");
    initializePlot(ui.syncPlot2, "Sync Plot 2");

    const auto singleMarker = new SingleMarker(ui.syncPlot2);
    singleMarker->setPen(ELS_Hovered, QPen(Qt::yellow));
    singleMarker->setAxis(EA_yAxis);
    singleMarker->setMarkerPos(50);

    QtPlot::syncZoom(ui.syncPlot1, ui.syncPlot2, EA_xAxis, ESR_Range);
    QtPlot::syncZoom(ui.syncPlot1, ui.syncPlot2, EA_yAxis, ESR_Range);

    QtPlot::syncMarkers(ui.syncPlot1, ui.syncPlot2, EA_xAxis);
    QtPlot::syncMarkers(ui.syncPlot1, ui.syncPlot2, EA_yAxis);
}

void MainWindow::initializePlot(QtPlot* plot, QString plotName)
{
    plot->setMouseTracking(true);
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // plot->yAxis->setLabel("y");
    plot->yAxis->setTickLabelColor(Qt::white);
    plot->yAxis->setLabelColor(Qt::white);
    plot->yAxis->setBasePen(QPen(Qt::white));

    // plot->xAxis->setLabel("x");
    plot->xAxis->setTickLabelColor(Qt::white);
    plot->xAxis->setLabelColor(Qt::white);
    plot->xAxis->setBasePen(QPen(Qt::white));

    plot->setBackground(Qt::black);
    QCPTextElement* chartSpectrRuntimeText = new QCPTextElement(plot);
    chartSpectrRuntimeText->setText(plotName);
    chartSpectrRuntimeText->setFont(QFont("sans", 11, QFont::Bold));
    chartSpectrRuntimeText->setTextColor(Qt::white);

    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0, chartSpectrRuntimeText);

    // Clamp View Zone
    {
        plot->setAxisClamp(EA_xAxis, true);
        plot->setAxisClamp(EA_yAxis, true);

        plot->setAxisClampRange(EA_xAxis, QCPRange(0, 100));
        plot->setAxisClampRange(EA_yAxis, QCPRange(0, 100));

        plot->xAxis->setRange(0, 100);
        plot->yAxis->setRange(0, 100);
    }

    // Setup Markers
    {
        plot->setMarkerAxisActive(EA_xAxis, true);
        plot->setMarkerMovableAxisActive(EA_xAxis, true);

        plot->setMarkerAxisActive(EA_yAxis, true);
        plot->setMarkerMovableAxisActive(EA_yAxis, true);

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
                plot->setMarkersPen(EA_xAxis, ELS_Idle, pen);
                plot->setMarkersPen(EA_xAxis, ELS_Hovered, hoveredPen);
                plot->setMarkersPen(EA_xAxis, ELS_Dragging, pen);

                plot->setMidMarkerPen(EA_xAxis, ELS_Idle, midPen);
                plot->setMidMarkerPen(EA_xAxis, ELS_Hovered, hoveredPen);
                plot->setMidMarkerPen(EA_xAxis, ELS_Dragging, pen);
                plot->setTextColor(EA_xAxis, Qt::white);
            }

            {
                plot->setMarkersPen(EA_yAxis, ELS_Idle, pen);
                plot->setMarkersPen(EA_yAxis, ELS_Hovered, hoveredPen);
                plot->setMarkersPen(EA_yAxis, ELS_Dragging, pen);

                plot->setMidMarkerPen(EA_yAxis, ELS_Idle, midPen);
                plot->setMidMarkerPen(EA_yAxis, ELS_Hovered, hoveredPen);
                plot->setMidMarkerPen(EA_yAxis, ELS_Dragging, pen);
                plot->setTextColor(EA_yAxis, Qt::white);
            }
        }
    }
}
