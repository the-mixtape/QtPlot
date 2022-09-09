#include "WaterfallLayer.h"

#include "ColorMap/WfColorMap.h"

WaterfallLayer::WaterfallLayer()
{
	colorMap = nullptr;
	fillColor = Qt::white;
}

WaterfallLayer::~WaterfallLayer()
{
	delete colorMap;
}
