#include "MovableItemLine.h"


MovableItemLine::MovableItemLine(QCustomPlot* parentPlot, QCPItemText* text /*= nullptr*/)
: QCPItemLine(parentPlot), attachedText(text)
{
	connect(parentPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent*)));
	connect(parentPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));

	setSelectable(true);
}

MovableItemLine::~MovableItemLine()
{
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
