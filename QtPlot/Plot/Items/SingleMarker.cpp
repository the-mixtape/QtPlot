#include "SingleMarker.h"
#include "Library/QtPlotMathLibrary.h"


SingleMarker::SingleMarker(QCustomPlot* parentPlot)
	:BaseMarker(parentPlot)
{
	QPen pen;
	pen.setColor(Qt::red);
	pen.setWidth(2);
	QCPItemStraightLine::setPen(pen);

	setPen(ELS_Idle, pen);
	setPen(ELS_Hovered, pen);
	setPen(ELS_Dragging, pen);
}

void SingleMarker::setMarkerPos(int pos)
{
	if (getAxis() == EA_xAxis)
	{
		if (equals(point1->coords().x(), pos)) return;

		point1->setCoords(pos, 0);
		point2->setCoords(pos, 1);
	}
	else
	{
		if (equals(point1->coords().y(), pos)) return;

		point1->setCoords(0, pos);
		point2->setCoords(1, pos);
	}

	showMarker();
	replot();

	emit updateMarkerPosSignal(pos);
}

void SingleMarker::showMarker()
{
	setVisible(true);
}

void SingleMarker::hideMarker()
{
	setVisible(false);
}

void SingleMarker::xMoveAxis(QMouseEvent* event)
{
	double newPos = mParentPlot->xAxis->pixelToCoord(event->pos().x());

	const auto clampRange = mParentPlot->xAxis->range();

	if (newPos < clampRange.lower)
	{
		newPos = clampRange.lower;
	}

	if (newPos > clampRange.upper)
	{
		newPos = clampRange.upper;
	}
	
	if (equals(newPos, point1->coords().x()) == false)
	{
		setMarkerPos(newPos);
	}
}

void SingleMarker::yMoveAxis(QMouseEvent* event)
{
	double newPos = mParentPlot->yAxis->pixelToCoord(event->pos().y());
	auto clampRange = mParentPlot->yAxis->range();

	if (newPos < clampRange.lower)
	{
		newPos = clampRange.lower;
	}

	if (newPos > clampRange.upper)
	{
		newPos = clampRange.upper;
	}

	if (equals(newPos, point1->coords().y()) == false)
	{
		setMarkerPos(newPos);
	}
}
