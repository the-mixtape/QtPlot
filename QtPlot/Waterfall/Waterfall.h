#pragma once

#include "Interval.h"
#include "WaterfallThread.h"
#include "Plot/QtPlot.h"
#include "Waterfall/WaterfallContent.h"


//forward declaration
class WaterfallThread;
class WaterfallContent;


class QTPLOT_EXPORT WaterfallPlot : public QtPlot
{
	Q_OBJECT

public:
	explicit WaterfallPlot(QWidget* parent);
	~WaterfallPlot() override;

	virtual void appendData(double* data, int size) const;
	virtual void setData(double* data, int width, int height) const;

	virtual void clear();

	void updateWaterfall();

	void setAutoUpdate(bool bAuto = true);
	void setFPSLimit(quint32 fps = 0) const;
	void setColorMap(WfColorMap* colorMap) const;
	void setAppendSide(EAppendSide side);;
	void setAppendHeight(int h) const;
	void setResolution(int width, int height) const;
	void setWidth(int width) const;
	void setHeight(int height) const;
	void setPosition(int minx, int miny, int maxx, int maxy);
	void setPositionX(int minx, int maxx) const;
	void setPositionY(int miny, int maxy) const;
	void setInterval(int minval, int maxval) const;

	inline bool getAutoUpdate() const { return loadThread->getAutoUpdate(); }
	inline quint32 getFPSLimit() const { return loadThread->getFPSLimit(); }
	inline WfColorMap* getColorMap() const { return content->getColorMap(); }
	inline QRect getResolution() const { return content->getResolution(); }

	QtInterval getInterval() const;

	void setFillColor(const QColor& fillColor) const;

signals:
	void copyingCompleted();

private:
	WaterfallThread* loadThread;
	WaterfallContent* content;

};

