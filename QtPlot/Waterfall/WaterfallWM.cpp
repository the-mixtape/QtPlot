#include "WaterfallWM.h"

#include "WaterfallContentWM.h"
#include "ColorMap/WaterfallColorMap.h"

WaterfallWithMemory::WaterfallWithMemory(QWidget* parent)
: WaterfallBase(parent)
{
	content = new WaterfallContentWithMemory(this);
	content->createLayer(200, 200, 0, 0, 100, 100, 0, 100, QImage::Format_ARGB32, Qt::white);
	content->setColorMap(new WaterfallColorMap());
	content->setLayer(WATERFALL_LAYER_NAME);

	loadThread->setWaterfallContent(content);
	loadThread->start();
}

WaterfallWithMemory::~WaterfallWithMemory()
{
}
