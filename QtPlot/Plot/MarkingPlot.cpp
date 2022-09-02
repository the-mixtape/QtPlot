#include "MarkingPlot.h"

#include "Items/MovableItemLine.h"
#include "Plot/Items/MovableInfinityLine.h"

MarkingPlot::MarkingPlot(QWidget* parent) : ZoomClampedPlot(parent)
{
	markerAttachRules.insert(EAxis::EA_xAxis, false);
	markerAttachRules.insert(EAxis::EA_yAxis, false);
	markerActiveRules.insert(EA_xAxis, false);
	markerActiveRules.insert(EA_yAxis, false);

	addLayer("markers");
	QCPLayer* markerLayer = layer("markers");
	markerLayer->setMode(QCPLayer::lmBuffered);

	initializeRangeLine(&horRangeLine1, EA_xAxis);
	initializeRangeLine(&horRangeLine2, EA_xAxis);
	initializeHorMarkerText(&horLineText);
	initializeLine(&horLine, horRangeLine1, horRangeLine2, EA_yAxis, horLineText);

	initializeRangeLine(&vertRangeLine1, EA_yAxis);
	initializeRangeLine(&vertRangeLine2, EA_yAxis);
	initializeVertMarkerText(&vertLineText);
	initializeLine(&vertLine, vertRangeLine1, vertRangeLine2, EA_xAxis, vertLineText);
}

MarkingPlot::~MarkingPlot()
{
}

void MarkingPlot::setMarkerAxisAttach(EAxis axis, bool isNeedAttach)
{
	markerAttachRules[axis] = isNeedAttach;
}

void MarkingPlot::setMarkerAxisActive(EAxis axis, bool isActive)
{
	markerActiveRules[axis] = isActive;

	if(isActive == false)
	{
		clearMarkers(axis);
	}
}

void MarkingPlot::setMarkerMovableAxisActive(EAxis axis, bool isActive) const
{
	if(axis == EA_xAxis)
	{
		horRangeLine1->setMovable(isActive);
		horRangeLine2->setMovable(isActive);
	}
	else
	{
		vertRangeLine1->setMovable(isActive);
		vertRangeLine2->setMovable(isActive);
	}
}

void MarkingPlot::clearMarkers(EAxis axis)
{
	if(axis == EA_xAxis)
	{
		horClickCount = 0;
		horRangeLine1->setVisible(false);
		horRangeLine2->setVisible(false);
		horLine->setVisible(false);
		horLineText->setVisible(false);
		layer("markers")->replot();
	}
	else
	{
		vertClickCount = 0;
		vertRangeLine1->setVisible(false);
		vertRangeLine2->setVisible(false);
		vertLine->setVisible(false);
		vertLineText->setVisible(false);
		layer("markers")->replot();
	}
}

void MarkingPlot::setMarkersPen(EAxis axis, ELineState state, QPen pen)
{
	if(axis == EA_xAxis)
	{
		horRangeLine1->setPen(state, pen);
		horRangeLine2->setPen(state, pen);
	}
	else
	{
		vertRangeLine1->setPen(state, pen);
		vertRangeLine2->setPen(state, pen);
	}
}

void MarkingPlot::setMidMarkerPen(EAxis axis, ELineState state, QPen pen)
{
	if (axis == EA_xAxis)
	{
		horLine->setPen(state, pen);
	}
	else
	{
		vertLine->setPen(state, pen);
	}
}

void MarkingPlot::setTextFont(EAxis axis, QFont font)
{
	if(axis == EA_xAxis)
	{
		horLineText->setFont(font);
	}
	else
	{
		vertLineText->setFont(font);
	}
}

void MarkingPlot::setTextColor(EAxis axis, QColor color)
{
	if (axis == EA_xAxis)
	{
		horLineText->setColor(color);
	}
	else
	{
		vertLineText->setColor(color);
	}
}

void MarkingPlot::setSetupHorLineTextFunc(QString(*func)(double start, double end))
{
	setupHorLineFunc = func;
}

void MarkingPlot::setSetupVertLineTextFunc(QString(*func)(double start, double end))
{
	setupVertLineFunc = func;
}

void MarkingPlot::mousePressEvent(QMouseEvent* event)
{
	QCustomPlot::mousePressEvent(event);

	mouseClickPoint = event->pos();
}

void MarkingPlot::mouseReleaseEvent(QMouseEvent* event)
{
	QCustomPlot::mouseReleaseEvent(event);
	
	if (mouseClickPoint != event->pos()) return;

	if (event->button() == horMouseButton && markerActiveRules[EA_xAxis])
	{
		horizontalClickEvent(event);
		return;
	}

	if (event->button() == vertMouseButton && markerActiveRules[EA_yAxis])
	{
		verticalClickEvent(event);
	}
}

void MarkingPlot::horizontalClickEvent(QMouseEvent* event)
{
	double x = xAxis->pixelToCoord(event->pos().x());
	const bool bNeedAttachMarker = markerAttachRules[EAxis::EA_xAxis];

	if (bNeedAttachMarker && graphCount() >= 0)
	{
		for (auto point = graph(0)->data()->constBegin(); point != graph(0)->data()->constEnd(); ++point)
		{
			if (fabs(point->key - x) < 0.5) {
				x = point->key;
				break;
			}
		}
	}

	if (horClickCount == 0)
	{
		setHorLineCoords(horRangeLine1, x);
	}

	if (horClickCount == 1)
	{
		setHorLineCoords(horRangeLine2, x);

		// setup idle mid line position
		{
			const auto currentRange = yAxis->range();
			const double y = currentRange.upper - ((currentRange.upper - currentRange.lower) / 2.0);

			horLine->start->setCoords(horLine->start->coords().x(), y);
			horLine->end->setCoords(horLine->start->coords().x(), horLine->start->coords().y());
		}

		updateHorizontalMarkers();

		horLineText->setVisible(true);
		horLine->setVisible(true);

		layer("markers")->replot();

		double firstPoint = horRangeLine1->point1->key();
		double secondPoint = horRangeLine2->point1->key();

		if (firstPoint > secondPoint)
		{
			const double temp = firstPoint;
			firstPoint = secondPoint;
			secondPoint = temp;
		}
		emit rangeHorSelectedSignal(firstPoint, secondPoint);
	}

	if (horClickCount == 2)
	{
		horRangeLine1->setVisible(false);
		horRangeLine2->setVisible(false);
		horLine->setVisible(false);
		horLineText->setVisible(false);
		layer("markers")->replot();
	}

	incrementCount(horClickCount);
}

void MarkingPlot::verticalClickEvent(QMouseEvent* event)
{
	double y = yAxis->pixelToCoord(event->pos().y());
	const bool bNeedAttachMarker = markerAttachRules[EAxis::EA_yAxis];

	if (bNeedAttachMarker && graphCount() >= 0)
	{
		for (auto point = graph(0)->data()->constBegin(); point != graph(0)->data()->constEnd(); ++point)
		{
			if (fabs(point->value - y) < 0.5) {
				y = point->value;
				break;
			}
		}
	}

	if (vertClickCount == 0)
	{
		setVertLineCoords(vertRangeLine1, y);
	}

	if (vertClickCount == 1)
	{
		setVertLineCoords(vertRangeLine2, y);

		// setup idle mid line position
		{
			const auto currentRange = xAxis->range();
			const double x = currentRange.lower + ((currentRange.upper - currentRange.lower) / 5.0);

			vertLine->start->setCoords(x, vertLine->start->coords().y());
			vertLine->end->setCoords(x, vertLine->start->coords().y());
		}

		updateVerticalMarkers();

		vertLine->setVisible(true);
		vertLineText->setVisible(true);

		layer("markers")->replot();

		double firstPoint = vertRangeLine1->point1->value();
		double secondPoint = vertRangeLine2->point1->value();

		if (firstPoint > secondPoint)
		{
			const double temp = firstPoint;
			firstPoint = secondPoint;
			secondPoint = temp;
		}
		emit rangeHorSelectedSignal(firstPoint, secondPoint);
	}

	if (vertClickCount == 2)
	{
		vertRangeLine1->setVisible(false);
		vertRangeLine2->setVisible(false);
		vertLine->setVisible(false);
		vertLineText->setVisible(false);
		layer("markers")->replot();
	}

	incrementCount(vertClickCount);
}

void MarkingPlot::initializeRangeLine(MovableInfinityLine** line, EAxis moveAxis)
{
	(*line) = new MovableInfinityLine(this);
	(*line)->setVisible(false);
	(*line)->setLayer("markers");
	(*line)->setMoveAxis(moveAxis);

	if(moveAxis == EA_xAxis)
	{
		connect((*line), SIGNAL(updatePosition()), this, SLOT(updateHorizontalMarkers()));
	}
	else
	{
		connect((*line), SIGNAL(updatePosition()), this, SLOT(updateVerticalMarkers()));
	}
}

void MarkingPlot::initializeLine(MovableItemLine** line, MovableInfinityLine* fMarker, MovableInfinityLine* sMarker, EAxis moveAxis, QCPItemText* text)
{
	(*line) = new MovableItemLine(this, fMarker, sMarker, text);
	(*line)->setVisible(false);
	(*line)->setLayer("markers");
	(*line)->setMoveAxis(moveAxis);
}

void MarkingPlot::initializeHorMarkerText(QCPItemText** text)
{
	(*text) = new QCPItemText(this);
	(*text)->setPositionAlignment(Qt::AlignTop | Qt::AlignHCenter);
	(*text)->position->setType(QCPItemPosition::ptPlotCoords);
	(*text)->setFont(QFont(font().family(), 12));
	(*text)->setColor(Qt::black);
	(*text)->setVisible(false);
	(*text)->setLayer("markers");
}

void MarkingPlot::initializeVertMarkerText(QCPItemText** text)
{
	(*text) = new QCPItemText(this);
	(*text)->setPositionAlignment(Qt::AlignVCenter);
	(*text)->position->setType(QCPItemPosition::ptPlotCoords);
	(*text)->setFont(QFont(font().family(), 12));
	(*text)->setColor(Qt::black);
	(*text)->setVisible(false);
	(*text)->setLayer("markers");
}

void MarkingPlot::incrementCount(short& count)
{
	count++;
	if (count >= 3)
	{
		count = 0;
	}
}

void MarkingPlot::setHorLineCoords(MovableInfinityLine* line, double x)
{
	line->setPoint1Coord(x, 0);
	line->setPoint2Coord(x, 1);
	line->setVisible(true);

	layer("markers")->replot();
}

void MarkingPlot::setVertLineCoords(MovableInfinityLine* line, double y)
{
	line->setPoint1Coord(0, y);
	line->setPoint2Coord(1, y);
	line->setVisible(true);

	layer("markers")->replot();
}

void MarkingPlot::updateHorizontalMarkers()
{
	double firstPoint = horRangeLine1->point1->key();
	double secondPoint = horRangeLine2->point1->key();

	if (firstPoint > secondPoint)
	{
		const double temp = firstPoint;
		firstPoint = secondPoint;
		secondPoint = temp;
	}
	
	horLine->start->setCoords(firstPoint, horLine->start->coords().y());
	horLine->end->setCoords(secondPoint, horLine->start->coords().y());

	const double midX = secondPoint - ((secondPoint - firstPoint) / 2.0);

	horLineText->position->setCoords(midX, horLine->start->coords().y());
	horLineText->setText(setupHorizontalText(firstPoint, secondPoint));
}

void MarkingPlot::updateVerticalMarkers()
{
	double firstPoint = vertRangeLine1->point1->value();
	double secondPoint = vertRangeLine2->point1->value();

	if (firstPoint > secondPoint)
	{
		const double temp = firstPoint;
		firstPoint = secondPoint;
		secondPoint = temp;
	}

	vertLine->start->setCoords(vertLine->start->coords().x(), firstPoint);
	vertLine->end->setCoords(vertLine->start->coords().x(), secondPoint);

	const double midY = secondPoint - ((secondPoint - firstPoint) / 2.0);
	vertLineText->position->setCoords(vertLine->start->coords().x(), midY);
	vertLineText->setText(setupVerticalText(firstPoint, secondPoint));
}