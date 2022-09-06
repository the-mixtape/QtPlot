#include "Waterfall.h"

#include "WfColorMap.h"
#include "WaterfallContent.h"
#include "WaterfallThread.h"


WaterfallPlot::WaterfallPlot(QWidget* parent)
	:QtPlot(parent)
{
	addLayer(WATERFALL_LAYER_NAME, layer(MARKERS_LAYER_NAME), limBelow);

	content = new WaterfallContent(this);
	content->createLayer(512, 512, 0, 0, 100, 100, 0, 100, QImage::Format_ARGB32, Qt::white);
	content->setColorMap(new LinearColorMap(Qt::blue, Qt::yellow));
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

void WaterfallPlot::setColorMap(WfColorMap* colorMap)
{
	content->setColorMap(colorMap);
}

void WaterfallPlot::setAppendSide(EAppendSide side)
{
	content->setAppendSide(side);
}

void WaterfallPlot::setAppendHeight(int h)
{
	content->setAppendHeight(h);
}

void WaterfallPlot::setResolution(int width, int height)
{
	content->setResolution(width, height);
}

void WaterfallPlot::setPosition(int minx, int miny, int maxx, int maxy)
{
	setPositionX(minx, maxx);
	setPositionY(miny, maxy);
}

void WaterfallPlot::setPositionX(int minx, int maxx)
{
	xAxis->setRange(minx, maxx);
	content->setPositionX(minx, maxx);
}

void WaterfallPlot::setPositionY(int miny, int maxy)
{
	yAxis->setRange(miny, maxy);
	content->setPositionY(miny, maxy);
}

void WaterfallPlot::setInterval(int minval, int maxval)
{
	content->setInterval(minval, maxval);
}

void WaterfallPlot::setFillColor(QColor fillColor)
{
	content->setFillColor(fillColor);
}

void WaterfallPlot::clear()
{
	content->clear();
}
