#include "MovableItemLine.h"

#include "Plot/Items/MovableInfinityLine.h"


MovableItemLine::MovableItemLine(QCustomPlot* parentPlot, MovableInfinityLine* fMarker, MovableInfinityLine* sMarker, QCPItemText* text /*= nullptr*/)
	: QCPItemLine(parentPlot),
	firstMarker(fMarker), secondMarker(sMarker),
	attachedText(text)
{
	//initialize default pens
	{
		QPen pen;
		pen.setWidth(2);
		QCPItemLine::setPen(pen);

		pens[idle] = pen;
		pens[hovered] = pen;
		pens[dragging] = pen;
	}

	fMarker->setMidLine(this);
	sMarker->setMidLine(this);

	connect(parentPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent*)));
	connect(parentPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));

	setSelectable(true);
}

MovableItemLine::~MovableItemLine()
{
}

void MovableItemLine::updatePosition()
{
	if(axis == EA_xAxis)
	{
		yUpdate();
	}
	else
	{
		xUpdate();
	}
}

void MovableItemLine::setPen(ELineState inState, QPen pen)
{
	pens[inState] = pen;
	if (inState == idle) QCPItemLine::setPen(pen);
}

void MovableItemLine::mousePressEvent(QMouseEvent* event, const QVariant& details)
{
	QCPItemLine::mousePressEvent(event, details);
	
	setIsDrag(true);
}

void MovableItemLine::setIsDrag(bool drag)
{
	if (bIsDrag == drag) return;
	
	bIsDrag = drag;

	if(bIsDrag)
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

void MovableItemLine::xMoveAxis(QMouseEvent* event)
{
	const double newPos = mParentPlot->xAxis->pixelToCoord(event->pos().x());

	QPointF startCoords = start->coords();
	startCoords.setX(newPos);
	start->setCoords(startCoords);

	QPointF endCoords = end->coords();
	endCoords.setX(newPos);
	end->setCoords(endCoords);

	if (attachedText == nullptr) return;

	QPointF textCoords = attachedText->position->coords();
	textCoords.setX(newPos);
	attachedText->position->setCoords(textCoords);
}

void MovableItemLine::yMoveAxis(QMouseEvent* event)
{
	const double newPos = mParentPlot->yAxis->pixelToCoord(event->pos().y());

	QPointF startCoords = start->coords();
	startCoords.setY(newPos);
	start->setCoords(startCoords);

	QPointF endCoords = end->coords();
	endCoords.setY(newPos);
	end->setCoords(endCoords);

	if (attachedText == nullptr) return;

	QPointF textCoords = attachedText->position->coords();
	textCoords.setY(newPos);
	attachedText->position->setCoords(textCoords);
}

void MovableItemLine::checkHovered(QMouseEvent* event)
{
	const int mouseX = event->pos().x();
	const int mouseY = event->pos().y();
	const int width = pen().width();

	bool hoveredX;
	bool hoveredY;

	if (axis == EA_xAxis)
	{
		const int lineX = mParentPlot->xAxis->coordToPixel(start->coords().x());
		const int lineStartY = mParentPlot->yAxis->coordToPixel(start->coords().y());
		const int lineEndY = mParentPlot->yAxis->coordToPixel(end->coords().y());

		hoveredX = mouseX >= lineX - width && mouseX <= lineX + width;
		hoveredY = mouseY >= lineEndY && mouseY <= lineStartY;
	}
	else
	{
		const int lineY = mParentPlot->yAxis->coordToPixel(start->coords().y());
		const int lineStartX = mParentPlot->xAxis->coordToPixel(start->coords().x());
		const int lineEndX = mParentPlot->xAxis->coordToPixel(end->coords().x());

		hoveredX = mouseX >= lineStartX && mouseX <= lineEndX;
		hoveredY = mouseY >= lineY - width && mouseY <= lineY + width;
	}

	if (hoveredX && hoveredY)
	{
		setState(hovered);
	}
	else
	{
		setState(idle);
	}
}

void MovableItemLine::setState(ELineState inState)
{
	if (state == inState) return;

	QPen newPen = pen();
	bool needReplot = false;
	if (inState == hovered && state == idle)
	{
		state = hovered;
		newPen = pens[hovered];
		needReplot = true;
	}

	if (inState == dragging)
	{
		state = dragging;
		newPen = pens[dragging];
		needReplot = true;
	}

	if (inState == idle && !bIsDrag)
	{
		state = idle;
		newPen = pens[idle];
		needReplot = true;
	}

	if (needReplot) 
	{
		QCPItemLine::setPen(newPen);
		mParentPlot->layer("markers")->replot();
		// mParentPlot->replot();
	}
}

void MovableItemLine::xUpdate()
{
	QPointF leftMarker = firstMarker->point1->coords();
	QPointF rightMarker = secondMarker->point1->coords();

	if(leftMarker.x() > rightMarker.x())
	{
		const auto temp = leftMarker;
		leftMarker = rightMarker;
		rightMarker = temp;
	}

	QPointF startCoords = start->coords();
	startCoords.setX(leftMarker.x());
	start->setCoords(startCoords);

	QPointF endCoords = end->coords();
	endCoords.setX(rightMarker.x());
	end->setCoords(endCoords);

	if (attachedText == nullptr) return;

	double textCoord = leftMarker.x() + (rightMarker.x() - leftMarker.x()) / 2.0;
	QPointF textCoords = attachedText->position->coords();
	textCoords.setX(textCoord);
	attachedText->position->setCoords(textCoords);
}

void MovableItemLine::yUpdate()
{
	QPointF leftMarker = firstMarker->point1->coords();
	QPointF rightMarker = secondMarker->point1->coords();

	if (leftMarker.y() > rightMarker.y())
	{
		const auto temp = leftMarker;
		leftMarker = rightMarker;
		rightMarker = temp;
	}

	QPointF startCoords = start->coords();
	startCoords.setY(leftMarker.y());
	start->setCoords(startCoords);

	QPointF endCoords = end->coords();
	endCoords.setY(rightMarker.y());
	end->setCoords(endCoords);

	if (attachedText == nullptr) return;

	double textCoord = leftMarker.y() + (rightMarker.y() - leftMarker.y()) / 2.0;
	QPointF textCoords = attachedText->position->coords();
	textCoords.setY(textCoord);
	attachedText->position->setCoords(textCoords);
}

void MovableItemLine::mouseRelease(QMouseEvent* event)
{
	setIsDrag(false);
}

void MovableItemLine::mouseMove(QMouseEvent* event)
{
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

	mParentPlot->layer("markers")->replot();
}
