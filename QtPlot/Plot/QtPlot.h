#pragma once

#include "QtPlotGlobal.h"
#include "Plot/InfoPlot.h"


class QTPLOT_EXPORT QtPlot : public InfoPlot
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
