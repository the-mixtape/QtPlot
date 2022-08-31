#include "MarkingPlot.h"

#include "Items/MovableItemLine.h"

MarkingPlot::MarkingPlot(QWidget* parent) : ZoomClampedPlot(parent)
{
	markerAttachRules.insert(EAxis::EA_xAxis, false);
	markerAttachRules.insert(EAxis::EA_yAxis, false);
	markerActiveRules.insert(EA_xAxis, false);
	markerActiveRules.insert(EA_yAxis, false);

	initializeRangeLine(&horRangeLine1);
	initializeRangeLine(&horRangeLine2);
	initializeHorMarkerText(&horLineText);
	initializeLine(&horLine, EA_yAxis, horLineText);

	initializeRangeLine(&vertRangeLine1);
	initializeRangeLine(&vertRangeLine2);
	initializeVertMarkerText(&vertLineText);
	initializeLine(&vertLine, EA_xAxis, vertLineText);
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

void MarkingPlot::clearMarkers(EAxis axis)
{
	if(axis == EA_xAxis)
	{
		horClickCount = 0;
		horRangeLine1->setVisible(false);
		horRangeLine2->setVisible(false);
		horLine->setVisible(false);
		horLineText->setVisible(false);
		replot();
	}
	else
	{
		vertClickCount = 0;
		vertRangeLine1->setVisible(false);
		vertRangeLine2->setVisible(false);
		vertLine->setVisible(false);
		vertLineText->setVisible(false);
		replot();
	}
}

void MarkingPlot::setHorMarkersPen(QPen pen)
{
	horRangeLine1->setPen(pen);
	horRangeLine2->setPen(pen);
}

void MarkingPlot::setHorMidMarkerPen(QPen pen)
{
	horLine->setPen(pen);
}

void MarkingPlot::setHorTextFont(QFont font)
{
	horLineText->setFont(font);
}

void MarkingPlot::setHorTextColor(QColor color)
{
	horLineText->setColor(color);
}

void MarkingPlot::setVertMarkersPen(QPen pen)
{
	vertRangeLine1->setPen(pen);
	vertRangeLine2->setPen(pen);
}

void MarkingPlot::setVertMidMarkerPen(QPen pen)
{
	vertLine->setPen(pen);
}

void MarkingPlot::setVertTextFont(QFont font)
{
	vertLineText->setFont(font);
}

void MarkingPlot::setVertTextColor(QColor color)
{
	vertLineText->setColor(color);
}

void MarkingPlot::setPressLimitTime(qint64 msec)
{
	pressLimitTime = msec;
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

	timer.start();
}

void MarkingPlot::mouseReleaseEvent(QMouseEvent* event)
{
	QCustomPlot::mouseReleaseEvent(event);


	if (event->button() == horMouseButton && markerActiveRules[EA_xAxis])
	{
		const qint64 press_time = timer.elapsed();
		if (press_time > pressLimitTime) return;

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

		double firstPoint = horRangeLine1->point1->key();
		double secondPoint = horRangeLine2->point1->key();

		if (firstPoint > secondPoint)
		{
			const double temp = firstPoint;
			firstPoint = secondPoint;
			secondPoint = temp;
		}

		const auto currentRange = yAxis->range();
		const double y = currentRange.upper - ((currentRange.upper - currentRange.lower) / 2.0);

		horLine->start->setCoords(firstPoint, y);
		horLine->end->setCoords(secondPoint, y);
		horLine->setVisible(true);

		const double midX = secondPoint - ((secondPoint - firstPoint) / 2.0);

		horLineText->position->setCoords(midX, y);
		horLineText->setText(setupHorizontalText(firstPoint, secondPoint));
		horLineText->setVisible(true);

		replot();
		emit rangeHorSelectedSignal(firstPoint, secondPoint);
	}

	if (horClickCount == 2)
	{
		horRangeLine1->setVisible(false);
		horRangeLine2->setVisible(false);
		horLine->setVisible(false);
		horLineText->setVisible(false);
		replot();
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

		double firstPoint = vertRangeLine1->point1->value();
		double secondPoint = vertRangeLine2->point1->value();

		if (firstPoint > secondPoint)
		{
			const double temp = firstPoint;
			firstPoint = secondPoint;
			secondPoint = temp;
		}

		const auto currentRange = xAxis->range();
		const double x = currentRange.lower + ((currentRange.upper - currentRange.lower) / 5.0);

		vertLine->start->setCoords(x, firstPoint);
		vertLine->end->setCoords(x, secondPoint);
		vertLine->setVisible(true);

		const double midY = secondPoint - ((secondPoint - firstPoint) / 2.0);

		vertLineText->position->setCoords(x, midY);
		vertLineText->setText(setupVerticalText(firstPoint, secondPoint));
		vertLineText->setVisible(true);

		replot();
		emit rangeHorSelectedSignal(firstPoint, secondPoint);
	}

	if (vertClickCount == 2)
	{
		vertRangeLine1->setVisible(false);
		vertRangeLine2->setVisible(false);
		vertLine->setVisible(false);
		vertLineText->setVisible(false);
		replot();
	}

	incrementCount(vertClickCount);
}

void MarkingPlot::initializeRangeLine(QCPItemStraightLine** line)
{
	QPen pen = QPen(Qt::PenStyle::SolidLine);
	pen.setWidth(2);
	pen.setColor(Qt::black);

	*line = new QCPItemStraightLine(this);
	(*line)->setPen(pen);
	(*line)->setVisible(false);
	(*line)->setLayer("axes");
}

void MarkingPlot::initializeLine(MovableItemLine** line, EAxis moveAxis, QCPItemText* text)
{
	QPen pen = QPen(Qt::PenStyle::DashLine);
	pen.setWidth(1);
	pen.setColor(Qt::black);

	(*line) = new MovableItemLine(this, text);
	(*line)->setPen(pen);
	(*line)->setVisible(false);
	(*line)->setLayer("axes");
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
	(*text)->setLayer("axes");
}

void MarkingPlot::initializeVertMarkerText(QCPItemText** text)
{
	(*text) = new QCPItemText(this);
	(*text)->setPositionAlignment(Qt::AlignVCenter);
	(*text)->position->setType(QCPItemPosition::ptPlotCoords);
	(*text)->setFont(QFont(font().family(), 12));
	(*text)->setColor(Qt::black);
	(*text)->setVisible(false);
	(*text)->setLayer("axes");
}

void MarkingPlot::incrementCount(short& count)
{
	count++;
	if (count >= 3)
	{
		count = 0;
	}
}

void MarkingPlot::setHorLineCoords(QCPItemStraightLine* line, double x)
{
	line->point1->setCoords(x, 0);
	line->point2->setCoords(x, 1);
	line->setVisible(true);

	replot();
}

void MarkingPlot::setVertLineCoords(QCPItemStraightLine* line, double y)
{
	line->point1->setCoords(0, y);
	line->point2->setCoords(1, y);
	line->setVisible(true);

	replot();
}
