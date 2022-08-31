#include "MovableItemLine.h"

#include "Plot/Items/MovableInfinityLine.h"


MovableItemLine::MovableItemLine(QCustomPlot* parentPlot, MovableInfinityLine* fMarker, MovableInfinityLine* sMarker, QCPItemText* text /*= nullptr*/)
	: QCPItemLine(parentPlot),
	firstMarker(fMarker), secondMarker(sMarker),
	attachedText(text)
{
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
		currentInteractions = mParentPlot->interactions();

		int temp = currentInteractions;
		mParentPlot->setInteraction(static_cast<QCP::Interaction>(temp), false);
	}
	else
	{
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
	if (bIsDrag == false) return;

	if (axis == EA_xAxis)
	{
		xMoveAxis(event);
	}
	else
	{
		yMoveAxis(event);
	}

	mParentPlot->replot();
}
