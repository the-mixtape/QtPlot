#include "WaterfallLayer.h"

#include "WfColorMap.h"

WaterfallLayer::WaterfallLayer()
{
	colorMap = nullptr;
}

WaterfallLayer::~WaterfallLayer()
{
	delete colorMap;
}
