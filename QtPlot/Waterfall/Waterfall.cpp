#include "Waterfall.h"

#include "WaterfallThread.h"


WaterfallPlot::WaterfallPlot(QWidget* parent)
	:QtPlot(parent)
{



	loadThread = new WaterfallThread();
	loadThread->stopAndClear();
	// loadThread->setWaterfall(this);
	loadThread->start();
}

WaterfallPlot::~WaterfallPlot()
{

}
