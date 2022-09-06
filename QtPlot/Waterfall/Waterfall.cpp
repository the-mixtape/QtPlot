#include "Waterfall.h"

#include "WfColorMap.h"
#include "WaterfallContent.h"
#include "WaterfallThread.h"


WaterfallPlot::WaterfallPlot(QWidget* parent)
	:QtPlot(parent)
{
	addLayer(WATERFALL_LAYER_NAME, layer(MARKERS_LAYER_NAME), limBelow);

	content = new WaterfallContent(this);
	content->addLayer(512, 512, 0, 0, 100, 100, 0, 100, QImage::Format_ARGB32, Qt::black);
	WfColorMap* colorMap = new LinearColorMap(Qt::yellow, Qt::blue);
	content->setColorMap(colorMap);
	content->setLayer(WATERFALL_LAYER_NAME);

	layer(WATERFALL_LAYER_NAME)->replot();
	
	loadThread = new WaterfallThread();
	loadThread->stopAndClear();
	loadThread->setWaterfallContent(content);
	loadThread->start();
}

WaterfallPlot::~WaterfallPlot()
{
	loadThread->stopAndClear();
	delete loadThread;
}