#include "Waterfall.h"

#include "ColorMap/WaterfallColorMap.h"
#include "WaterfallContent.h"
#include "WaterfallThread.h"


WaterfallPlot::WaterfallPlot(QWidget* parent)
	:QtPlot(parent)
{
	addLayer(WATERFALL_LAYER_NAME, layer(MARKERS_LAYER_NAME), limBelow);

	content = new WaterfallContent(this);
	content->createLayer(200, 200, 0, 0, 100, 100, 0, 100, QImage::Format_ARGB32, Qt::white);
	content->setColorMap(new WaterfallColorMap());
	content->setLayer(WATERFALL_LAYER_NAME);

	xAxis->setRange(0, 100);
	yAxis->setRange(0, 100);

	xAxis->grid()->setSubGridVisible(false);
	yAxis->grid()->setSubGridVisible(false);

	replot();
	
	loadThread = new WaterfallThread();
	loadThread->stopAndClear();
	loadThread->setWaterfallContent(content);
	loadThread->start();

	connect(loadThread, &WaterfallThread::copyingCompleted, [=]() { emit copyingCompleted(); });
}

WaterfallPlot::~WaterfallPlot()
{
	loadThread->stopAndClear();
	delete loadThread;
}

void WaterfallPlot::appendData(double* data, int size) const
{
	loadThread->addData(data, size);
}

void WaterfallPlot::setData(double* data, int width, int height) const
{
	loadThread->setData(data, width, height);
}

void WaterfallPlot::clear()
{
	content->clear();
}

void WaterfallPlot::updateWaterfall()
{
	content->updatePixmap();
	content->update();
}

void WaterfallPlot::setAutoUpdate(bool bAuto /*= true*/)
{
	loadThread->setAutoUpdate(bAuto);
}

void WaterfallPlot::setFPSLimit(quint32 fps /*= 0*/) const
{
	loadThread->setFPSLimit(fps);
}

void WaterfallPlot::setColorMap(WfColorMap* colorMap) const
{
	content->setColorMap(colorMap);
}

auto WaterfallPlot::setAppendSide(EAppendSide side) -> void
{
	content->setAppendSide(side);
}

void WaterfallPlot::setAppendHeight(int h) const
{
	content->setAppendHeight(h);
}

void WaterfallPlot::setResolution(int width, int height) const
{
	content->setResolution(width, height);
}

void WaterfallPlot::setWidth(int width) const
{
	content->setWidth(width);
}

void WaterfallPlot::setHeight(int height) const
{
	content->setHeight(height);
}

void WaterfallPlot::setPosition(int minx, int miny, int maxx, int maxy)
{
	setPositionX(minx, maxx);
	setPositionY(miny, maxy);
}

void WaterfallPlot::setPositionX(int minx, int maxx) const
{
	xAxis->setRange(minx, maxx);
	content->setPositionX(minx, maxx);
}

void WaterfallPlot::setPositionY(int miny, int maxy) const
{
	yAxis->setRange(miny, maxy);
	content->setPositionY(miny, maxy);
}

void WaterfallPlot::setInterval(int minval, int maxval) const
{
	content->setInterval(minval, maxval);
}

QtInterval WaterfallPlot::getInterval() const
{
	return content->getInterval();
}

void WaterfallPlot::setFillColor(const QColor& fillColor) const
{
	content->setFillColor(fillColor);
}
