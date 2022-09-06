#include "WaterfallLayer.h"

#include "WfColorMap.h"

WaterfallLayer::WaterfallLayer()
{
	colorMap = nullptr;
	fillColor = Qt::white;
}

WaterfallLayer::~WaterfallLayer()
{
	delete colorMap;
}
