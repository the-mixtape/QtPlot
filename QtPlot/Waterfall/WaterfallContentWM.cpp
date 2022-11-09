#include "WaterfallContentWM.h"

#include "WaterfallLayer.h"
#include "ColorMap/WfColorMap.h"
#include "Library/QtPlotMathLibrary.h"

WaterfallContentWithMemory::WaterfallContentWithMemory(QCustomPlot* parent)
	:WaterfallContent(parent)
{
	wfData = new WaterfallData();
}

WaterfallContentWithMemory::~WaterfallContentWithMemory()
{
	delete wfData;
}

void WaterfallContentWithMemory::setInterval(int minval, int maxval)
{
	readWriteLock->lockForRead();
	{
		if (equals(minval, waterfallLayer->range.minValue())
			&& equals(maxval, waterfallLayer->range.maxValue()))
		{
			readWriteLock->unlock();
			return;
		}
	}
	readWriteLock->unlock();

	{
		readWriteLock->lockForWrite();
		waterfallLayer->range = QtInterval(minval, maxval);
		readWriteLock->unlock();

		// wfData->sortData();
		WaterfallContent::setData(wfData->data(), wfData->width(), wfData->offset());
	}
	
	update();
}

void WaterfallContentWithMemory::append(double* data, int size, bool needUpdatePixmap)
{
	wfData->append(data, size);
	WaterfallContent::append(data, size, needUpdatePixmap);
}

void WaterfallContentWithMemory::setData(double* data, int width, int height)
{
	wfData->setData(data, width, height);
	WaterfallContent::setData(data, width, height);
}

void WaterfallContentWithMemory::setResolution(int width, int height)
{
	wfData->initialize(width, height);
	WaterfallContent::setResolution(width, height);
}

void WaterfallContentWithMemory::clear()
{
	wfData->clear();
	WaterfallContent::clear();
}
