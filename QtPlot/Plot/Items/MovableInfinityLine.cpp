#include "MovableInfinityLine.h"
#include "MovableItemLine.h"


MovableInfinityLine::MovableInfinityLine(QCustomPlot* parentPlot)
	: QCPItemStraightLine(parentPlot)
{
	//initialize default pens
	{
		QPen pen;
		pen.setWidth(2);
		QCPItemStraightLine::setPen(pen);
	
		pens[idle] = pen;
		pens[hovered] = pen;
		pens[dragging] = pen;
	}

	connect(parentPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent*)));
	connect(parentPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));
}

MovableInfinityLine::~MovableInfinityLine()
{
}

void MovableInfinityLine::setPen(ELineState inState, QPen pen)
{
	pens[inState] = pen;
	if (inState == idle) QCPItemStraightLine::setPen(pen);
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
		setState(dragging);
		currentInteractions = mParentPlot->interactions();

		int temp = currentInteractions;
		mParentPlot->setInteraction(static_cast<QCP::Interaction>(temp), false);
	}
	else
	{
		setState(idle);
		mParentPlot->setInteractions(currentInteractions);
	}
}

void MovableInfinityLine::xMoveAxis(QMouseEvent* event)
{
	double newPos = mParentPlot->xAxis->pixelToCoord(event->pos().x());
	auto clampRange = mParentPlot->xAxis->range();

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
		setState(hovered);
	}
	else
	{
		setState(idle);
	}
}

void MovableInfinityLine::setState(ELineState inState)
{
	if (state == inState) return;

	QPen newPen = pen();
	bool needReplot = false;
	if(inState == hovered && state == idle)
	{
		state = hovered;
		newPen = pens[hovered];
		needReplot = true;
	}

	if(inState == dragging)
	{
		state = dragging;
		newPen = pens[dragging];
		needReplot = true;
	}

	if(inState == idle && !bIsDrag)
	{
		state = idle;
		newPen = pens[idle];
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
	checkHovered(event);

	if (!bIsMovable || bIsDrag == false) return;

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

	mParentPlot->layer("markers")->replot();
}

void MovableInfinityLine::axisXChanged(const QCPRange& range)
{
	qDebug() << "x changed";
}

void MovableInfinityLine::axisYChanged(const QCPRange& range)
{
	qDebug() << "y changed";
}
