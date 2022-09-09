#include "QtPlot.h"

QtPlot::QtPlot(QWidget* parent)
	: MarkingPlot(parent)
{
}

QtPlot::~QtPlot()
{
}

void QtPlot::syncZoom(QtPlot* plot1, QtPlot* plot2, EAxis syncAxis, ESyncRule syncRule)
{
	const auto plotAxis1 = syncAxis == EA_xAxis ? plot1->xAxis : plot1->yAxis;
	const auto plotAxis2 = syncAxis == EA_xAxis ? plot2->xAxis : plot2->yAxis;

	plot1->setSyncRule(syncAxis, syncRule);
	plot2->setSyncRule(syncAxis, syncRule);

	if (syncAxis == EA_xAxis)
	{
		connect(plotAxis1, SIGNAL(rangeChanged(QCPRange)), plot2, SLOT(setRangeX(QCPRange)));
		connect(plotAxis2, SIGNAL(rangeChanged(QCPRange)), plot1, SLOT(setRangeX(QCPRange)));
	}
	else
	{
		connect(plotAxis1, SIGNAL(rangeChanged(QCPRange)), plot2, SLOT(setRangeY(QCPRange)));
		connect(plotAxis2, SIGNAL(rangeChanged(QCPRange)), plot1, SLOT(setRangeY(QCPRange)));
	}
}
