#pragma once

#include "QtPlotGlobal.h"
#include "QCustomPlot/QCustomPlot.h"


class QTPLOT_EXPORT QtPlot : public QCustomPlot
{
public:
	explicit QtPlot(QWidget* parent = nullptr);
	virtual ~QtPlot();
};
