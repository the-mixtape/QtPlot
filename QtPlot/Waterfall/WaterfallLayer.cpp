#include "WaterfallLayer.h"

#include "ColorMap/WfColorMap.h"

WaterfallLayer::WaterfallLayer()
{
	colorMap = nullptr;
	fillColor = Qt::white;
	range = QtInterval(0, 0);
}

WaterfallLayer::~WaterfallLayer()
{
	delete colorMap;
}
