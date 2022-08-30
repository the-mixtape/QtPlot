#pragma once

#include "QtPlotGlobal.h"
#include "ZoomClampedPlot.h"


class QTPLOT_EXPORT QtPlot : public ZoomClampedPlot
{
	Q_OBJECT

public:
	explicit QtPlot(QWidget* parent = nullptr);
	virtual ~QtPlot();
};
