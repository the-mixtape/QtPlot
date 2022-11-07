#include "BaseMarker.h"

#include "Plot/MarkingPlot.h"

BaseMarker::BaseMarker(QCustomPlot* parentPlot)
	: QCPItemStraightLine(parentPlot)
{
	setLayer(MARKERS_LAYER_NAME);

	if(parentPlot)
	{
		markingPlot = dynamic_cast<MarkingPlot*>(parentPlot);
	}

	QPen pen;
	pen.setColor(Qt::white);
	pen.setWidth(2);
	QCPItemStraightLine::setPen(pen);

	pens[ELS_Idle] = pen;
	pens[ELS_Hovered] = pen;
	pens[ELS_Dragging] = pen;

	connect(parentPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent*)));
	connect(parentPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));
}

void BaseMarker::setPen(ELineState inState, QPen pen)
{
	pens[inState] = pen;
	if (inState == ELS_Idle) QCPItemStraightLine::setPen(pen);
}

void BaseMarker::replot()
{
	parentPlot()->layer(MARKERS_LAYER_NAME)->replot();
}

void BaseMarker::checkHovered(QMouseEvent* event)
{
	const int width = pen().width();
	int mousePos;
	int linePos;
	bool inRange;

	if (axis == EA_xAxis)
	{
		mousePos = event->pos().x();
		linePos = mParentPlot->xAxis->coordToPixel(point1->coords().x());

		const QCPRange yRange = mParentPlot->yAxis->range();
		const double yPos = mParentPlot->yAxis->pixelToCoord(event->pos().y());
		inRange = yPos > yRange.lower && yPos < yRange.upper;
	}
	else
	{
		mousePos = event->pos().y();
		linePos = mParentPlot->yAxis->coordToPixel(point1->coords().y());

		const QCPRange xRange = mParentPlot->xAxis->range();
		const double xPos = mParentPlot->xAxis->pixelToCoord(event->pos().x());
		inRange = xPos > xRange.lower && xPos < xRange.upper;
	}

	const bool isHovered = mousePos >= linePos - width && mousePos <= linePos + width && inRange;
	if (isHovered)
	{
		setState(ELS_Hovered);
	}
	else
	{
		setState(ELS_Idle);
	}
}

void BaseMarker::setState(ELineState inState)
{
	if (state == inState) return;

	QPen newPen = pen();
	bool needReplot = false;
	if (inState == ELS_Hovered && state == ELS_Idle)
	{
		state = ELS_Hovered;
		newPen = pens[ELS_Hovered];
		needReplot = true;
	}

	if (inState == ELS_Dragging)
	{
		state = ELS_Dragging;
		newPen = pens[ELS_Dragging];
		needReplot = true;
	}

	if (inState == ELS_Idle && !bIsDrag)
	{
		state = ELS_Idle;
		newPen = pens[ELS_Idle];
		needReplot = true;
	}

	if (needReplot)
	{
		QCPItemStraightLine::setPen(newPen);
		replot();
	}
}

void BaseMarker::setIsDrag(bool drag)
{
	if (bIsDrag == drag) return;

	if (markingPlot)
	{
		if (drag && markingPlot->getIsAlreadyDragging()) return;

		markingPlot->setIsAlreadyDragging(drag);
	}

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

void BaseMarker::mousePressEvent(QMouseEvent* event, const QVariant& details)
{
	QCPItemStraightLine::mousePressEvent(event, details);

	if (axis == EA_xAxis)
	{
		const QCPRange yRange = mParentPlot->yAxis->range();
		const double yPos = mParentPlot->yAxis->pixelToCoord(event->pos().y());
		if (yPos < yRange.lower || yPos > yRange.upper) return;
	}
	else
	{
		const QCPRange xRange = mParentPlot->xAxis->range();
		const double xPos = mParentPlot->xAxis->pixelToCoord(event->pos().x());
		if (xPos < xRange.lower || xPos > xRange.upper) return;
	}

	setIsDrag(true);
}

void BaseMarker::mouseRelease(QMouseEvent* event)
{
	setIsDrag(false);
}

bool BaseMarker::mouseMove(QMouseEvent* event)
{
	if (visible() == false) return false;

	checkHovered(event);

	if (getIsDrag() == false) return false;

	if (getAxis() == EA_xAxis)
	{
		xMoveAxis(event);
	}
	else
	{
		yMoveAxis(event);
	}

	return true;
}


