#pragma once

#include "Plot/QtPlot.h"

//forward declaration
class WaterfallThread;
class WaterfallContent;
class WfColorMap;

class QTPLOT_EXPORT WaterfallPlot : public QtPlot
{
	Q_OBJECT

public:
	explicit WaterfallPlot(QWidget* parent);
	~WaterfallPlot() override;

	void appendData(double* data, int size) const;
	void clear() const;

	void setFPSLimit(quint32 fps = 0) const;
	void setColorMap(WfColorMap* colorMap) const;
	void setAppendSide(EAppendSide side);;
	void setAppendHeight(int h) const;
	void setResolution(int width, int height) const;
	void setPosition(int minx, int miny, int maxx, int maxy);
	void setPositionX(int minx, int maxx) const;
	void setPositionY(int miny, int maxy) const;
	void setInterval(int minval, int maxval) const;
	void setFillColor(const QColor& fillColor) const;

signals:
	void copyingCompleted();

private:
	WaterfallThread* loadThread;
	WaterfallContent* content;

};

