#include "MovableInfinityLine.h"
#include "MovableItemLine.h"
#include "Library/QtPlotMathLibrary.h"


MovableInfinityLine::MovableInfinityLine(QCustomPlot* parentPlot)
	: QCPItemStraightLine(parentPlot),
	offset(0)
{
	//initialize default pens
	{
		QPen pen;
		pen.setWidth(2);
		QCPItemStraightLine::setPen(pen);
	
		pens[ELS_Idle] = pen;
		pens[ELS_Hovered] = pen;
		pens[ELS_Dragging] = pen;
	}

	connect(parentPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent*)));
	connect(parentPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));
}

MovableInfinityLine::~MovableInfinityLine()
{
}

void MovableInfinityLine::setPoint1Coord(double x, double y)
{
	point1->setCoords(x, y);
	realCoords = QPointF(x, y);
}

void MovableInfinityLine::setPoint2Coord(double x, double y)
{
	point2->setCoords(x, y);
	realCoords = QPointF(x, y);
}

void MovableInfinityLine::addOffset(int inOffset)
{
	offset = inOffset;

	auto coords1 = point1->coords();
	auto coords2 = point2->coords();

	qreal x1 = coords1.x();
	qreal x2 = coords2.x();
	qreal y1 = coords1.y();
	qreal y2 = coords2.y();

	if(axis == EA_xAxis)
	{
		x1 += offset;
		x2 += offset;
	}
	else
	{
		y1 += offset;
		y2 += offset;
	}

	setPoint1Coord(x1, y1);
	setPoint2Coord(x2, y2);
}

void MovableInfinityLine::setPen(ELineState inState, QPen pen)
{
	pens[inState] = pen;
	if (inState == ELS_Idle) QCPItemStraightLine::setPen(pen);
}

void MovableInfinityLine::setMoveAxis(EAxis inAxis)
{
	axis = inAxis;

	if(axis == EA_xAxis)
	{
		connect(mParentPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(axisXChanged(QCPRange)));
	}
	else
	{
		connect(mParentPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(axisYChanged(QCPRange)));
	}
}

void MovableInfinityLine::mousePressEvent(QMouseEvent* event, const QVariant& details)
{
	QCPItemStraightLine::mousePressEvent(event, details);

	if (!bIsMovable) return;

	setIsDrag(true);
}

void MovableInfinityLine::setIsDrag(bool drag)
{
	if (bIsDrag == drag) return;

	bIsDrag = drag;

	if (bIsDrag)
	{
		setState(ELS_Dragging);
		currentInteractions = mParentPlot->interactions();

		int temp = currentInteractions;
		mParentPlot->setInteraction(static_cast<QCP::Interaction>(temp), false);
	}
	else
	{
		setState(ELS_Idle);
		mParentPlot->setInteractions(currentInteractions);
	}
}

void MovableInfinityLine::xMoveAxis(QMouseEvent* event)
{
	double newPos = mParentPlot->xAxis->pixelToCoord(event->pos().x());

	const auto clampRange = mParentPlot->xAxis->range();

	if(newPos < clampRange.lower)
	{
		newPos = clampRange.lower;
	}

	if(newPos > clampRange.upper)
	{
		newPos = clampRange.upper;
	}

	QPointF startCoords = point1->coords();
	startCoords.setX(newPos);
	point1->setCoords(startCoords);

	QPointF endCoords = point2->coords();
	endCoords.setX(newPos);
	point2->setCoords(endCoords);

	realCoords.setX(newPos);
}

void MovableInfinityLine::yMoveAxis(QMouseEvent* event)
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

	QPointF startCoords = point1->coords();
	startCoords.setY(newPos);
	point1->setCoords(startCoords);

	QPointF endCoords = point2->coords();
	endCoords.setY(newPos);
	point2->setCoords(endCoords);

	realCoords.setY(newPos);
}

void MovableInfinityLine::checkHovered(QMouseEvent* event)
{
	const int width = pen().width();
	int mousePos;
	int linePos;

	if (axis == EA_xAxis)
	{
		mousePos = event->pos().x();
		linePos = mParentPlot->xAxis->coordToPixel(point1->coords().x());
	}
	else
	{
		mousePos = event->pos().y();
		linePos = mParentPlot->yAxis->coordToPixel(point1->coords().y());
	}

	const bool isHovered = mousePos >= linePos - width && mousePos <= linePos + width;
	if (isHovered)
	{
		setState(ELS_Hovered);
	}
	else
	{
		setState(ELS_Idle);
	}
}

void MovableInfinityLine::setState(ELineState inState)
{
	if (state == inState) return;

	QPen newPen = pen();
	bool needReplot = false;
	if(inState == ELS_Hovered && state == ELS_Idle)
	{
		state = ELS_Hovered;
		newPen = pens[ELS_Hovered];
		needReplot = true;
	}

	if(inState == ELS_Dragging)
	{
		state = ELS_Dragging;
		newPen = pens[ELS_Dragging];
		needReplot = true;
	}

	if(inState == ELS_Idle && !bIsDrag)
	{
		state = ELS_Idle;
		newPen = pens[ELS_Idle];
		needReplot = true;
	}

	if (needReplot)
	{
		QCPItemStraightLine::setPen(newPen);
		mParentPlot->layer("markers")->replot();
	}
}

void MovableInfinityLine::mouseRelease(QMouseEvent* event)
{
	setIsDrag(false);
}

void MovableInfinityLine::mouseMove(QMouseEvent* event)
{
	if (bIsMovable == false) return;

	checkHovered(event);

	if (bIsDrag == false) return;

	if (axis == EA_xAxis)
	{
		xMoveAxis(event);
	}
	else
	{
		yMoveAxis(event);
	}

	midLine->updatePosition();
	emit updatePosition();

	mParentPlot->layer(MARKERS_LAYER_NAME)->replot();
}

void MovableInfinityLine::axisXChanged(const QCPRange& range)
{
	if (bIsMovable == false) return;

	const double linePos = point1->coords().x();
	double newLinePos = linePos;

	const double locOffset = abs(range.upper - range.lower) * 0.003;

	bool clampMin = false;
	if (newLinePos < range.lower)
	{
		newLinePos = range.lower + locOffset;
		clampMin = true;
	}
	else if(equals(newLinePos, realCoords.x()) == false)
	{
		if(realCoords.x() < range.lower)
		{
			clampMin = true;
			newLinePos = range.lower + locOffset;
		}
		else
		{
			newLinePos = realCoords.x();
		}
	}

	if (clampMin == false)
	{
		if (newLinePos > range.upper)
		{
			newLinePos = range.upper - locOffset;
		}
		else if (equals(newLinePos, realCoords.x()) == false)
		{
			if (realCoords.x() < range.upper)
			{
				newLinePos = range.upper + locOffset;
			}
			else
			{
				newLinePos = realCoords.x();
			}
		}
	}

	if (equals(newLinePos, linePos) == false)
	{
		QPointF coords = point1->coords();
		coords.setX(newLinePos);
		point1->setCoords(coords);

		coords = point2->coords();
		coords.setX(newLinePos);
		point2->setCoords(coords);
		
		mParentPlot->layer("markers")->replot();
	}
}

void MovableInfinityLine::axisYChanged(const QCPRange& range)
{
	if (bIsMovable == false) return;

	const double linePos = point1->coords().y();
	double newLinePos = linePos;

	const double locOffset = abs(range.upper - range.lower) * 0.003;

	bool clampMin = false;
	if (newLinePos < range.lower)
	{
		newLinePos = range.lower + locOffset;
		clampMin = true;
	}
	else if (equals(newLinePos, realCoords.y()) == false)
	{
		if (realCoords.y() < range.lower)
		{
			clampMin = true;
			newLinePos = range.lower + locOffset;
		}
		else
		{
			newLinePos = realCoords.y();
		}
	}

	if (clampMin == false)
	{
		if (newLinePos > range.upper)
		{
			newLinePos = range.upper - locOffset;
		}
		else if (equals(newLinePos, realCoords.y()) == false)
		{
			if (realCoords.y() < range.upper)
			{
				newLinePos = range.upper + locOffset;
			}
			else
			{
				newLinePos = realCoords.y();
			}
		}
	}

	if (equals(newLinePos, linePos) == false)
	{
		QPointF coords = point1->coords();
		coords.setY(newLinePos);
		point1->setCoords(coords);

		coords = point2->coords();
		coords.setY(newLinePos);
		point2->setCoords(coords);
		
		mParentPlot->layer("markers")->replot();
	}
}
