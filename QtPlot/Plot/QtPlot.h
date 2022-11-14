#pragma once

#include "ClickablePlot.h"
#include "QtPlotGlobal.h"


class QTPLOT_EXPORT QtPlot : public ClickablePlot
{
	Q_OBJECT

public:
	explicit QtPlot(QWidget* parent = nullptr);
	virtual ~QtPlot();

	static void syncZoom(QtPlot* plot1, QtPlot* plot2, EAxis syncAxis, ESyncRule syncRule);
	static void desyncZoom(QtPlot* plot1, QtPlot* plot2, EAxis syncAxis);

	static void syncMarkers(QtPlot* plot1, QtPlot* plot2, EAxis syncAxis);
	static void desyncMarkers(QtPlot* plot1, QtPlot* plot2, EAxis syncAxis);
};
