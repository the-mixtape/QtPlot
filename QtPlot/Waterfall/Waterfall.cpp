#include "Waterfall.h"
#include "WaterfallContent.h"
#include "ColorMap/WaterfallColorMap.h"


WaterfallPlot::WaterfallPlot(QWidget* parent)
	: WaterfallBase(parent)
{
	content = new WaterfallContent(this);
	content->createLayer(200, 200, 0, 0, 100, 100, 0, 100, QImage::Format_ARGB32, Qt::white);
	content->setColorMap(new WaterfallColorMap());
	content->setLayer(WATERFALL_LAYER_NAME);

	loadThread->setWaterfallContent(content);
	loadThread->start();
}

WaterfallPlot::~WaterfallPlot()
{
}
