#pragma once

#include "QtPlotGlobal.h"
#include "MarkingPlot.h"


class QTPLOT_EXPORT QtPlot : public MarkingPlot
{
	Q_OBJECT

public:
	explicit QtPlot(QWidget* parent = nullptr);
	virtual ~QtPlot();
};
