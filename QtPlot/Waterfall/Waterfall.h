#pragma once

#include "Plot/QtPlot.h"

//forward declaration
class WaterfallThread;
class WaterfallContent;
class WfColorMap;

class QTPLOT_EXPORT WaterfallPlot : public QtPlot
{
public:
	explicit WaterfallPlot(QWidget* parent);
	~WaterfallPlot() override;

	void setFPSLimit(quint32 fps = 0);

	void setColorMap(WfColorMap* colorMap);
	void setAppendSide(EAppendSide side);;
	void setAppendHeight(int h);
	void setResolution(int width, int height);
	void setPosition(int minx, int miny, int maxx, int maxy);
	void setPositionX(int minx, int maxx);
	void setPositionY(int miny, int maxy);
	void setInterval(int minval, int maxval);
	void setFillColor(QColor fillColor);

	void clear();

private:
	WaterfallThread* loadThread;
	WaterfallContent* content;

};

