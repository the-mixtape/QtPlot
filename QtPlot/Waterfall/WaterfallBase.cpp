#include "WaterfallBase.h"

#include "ColorMap/WaterfallColorMap.h"
#include "WaterfallContent.h"
#include "WaterfallThread.h"


WaterfallBase::WaterfallBase(QWidget* parent)
	: QtPlot(parent)
{
	addLayer(WATERFALL_LAYER_NAME, layer(MARKERS_LAYER_NAME), limBelow);

	xAxis->setRange(0, 100);
	yAxis->setRange(0, 100);

	xAxis->grid()->setSubGridVisible(false);
	yAxis->grid()->setSubGridVisible(false);

	replot();
	
	loadThread = new WaterfallThread();
	loadThread->stopAndClear();

	connect(loadThread, &WaterfallThread::copyingCompleted, [=]() { emit copyingCompleted(); });
}

WaterfallBase::~WaterfallBase()
{
	loadThread->stopAndClear();
	delete loadThread;
}

void WaterfallBase::appendData(double* data, int size) const
{
	loadThread->addData(data, size);
}

void WaterfallBase::setData(double* data, int width, int height) const
{
	loadThread->setData(data, width, height);
}

void WaterfallBase::clear()
{
	content->clear();
}

void WaterfallBase::updateWaterfall()
{
	content->updatePixmap();
	content->update();
}

void WaterfallBase::setAutoUpdate(bool bAuto /*= true*/)
{
	loadThread->setAutoUpdate(bAuto);
}

void WaterfallBase::setFPSLimit(quint32 fps /*= 0*/) const
{
	loadThread->setFPSLimit(fps);
}

void WaterfallBase::setColorMap(WfColorMap* colorMap) const
{
	content->setColorMap(colorMap);
}

auto WaterfallBase::setAppendSide(EAppendSide side) -> void
{
	content->setAppendSide(side);
}

void WaterfallBase::setAppendHeight(int h) const
{
	content->setAppendHeight(h);
}

void WaterfallBase::setResolution(int width, int height) const
{
	content->setResolution(width, height);
}

void WaterfallBase::setWidth(int width) const
{
	content->setWidth(width);
}

void WaterfallBase::setHeight(int height) const
{
	content->setHeight(height);
}

void WaterfallBase::setPosition(int minx, int miny, int maxx, int maxy)
{
	setPositionX(minx, maxx);
	setPositionY(miny, maxy);
}

void WaterfallBase::setPositionX(int minx, int maxx) const
{
	xAxis->setRange(minx, maxx);
	content->setPositionX(minx, maxx);
}

void WaterfallBase::setPositionY(int miny, int maxy) const
{
	yAxis->setRange(miny, maxy);
	content->setPositionY(miny, maxy);
}

void WaterfallBase::setInterval(int minval, int maxval) const
{
	content->setInterval(minval, maxval);
}

QtInterval WaterfallBase::getInterval() const
{
	return content->getInterval();
}

void WaterfallBase::setFillColor(const QColor& fillColor) const
{
	content->setFillColor(fillColor);
}
