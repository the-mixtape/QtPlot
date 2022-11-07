#include "MarkingPlot.h"

#include "Items/MovableItemLine.h"
#include "Plot/Items/MovableInfinityLine.h"

MarkingPlot::MarkingPlot(QWidget* parent) : ZoomClampedPlot(parent)
{
	markerAttachRules.insert(EAxis::EA_xAxis, false);
	markerAttachRules.insert(EAxis::EA_yAxis, false);
	markerActiveRules.insert(EA_xAxis, false);
	markerActiveRules.insert(EA_yAxis, false);

	addLayer(MARKERS_LAYER_NAME);
	QCPLayer* markerLayer = layer(MARKERS_LAYER_NAME);
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

void MarkingPlot::addMarkersOffset(EAxis axis, int offset) const
{
	if (axis == EA_xAxis)
	{
		horRangeLine1->addOffset(offset);
		horRangeLine2->addOffset(offset);
		horLine->updatePosition();

		for(const auto& syncPlot: xSyncPlots)
		{
			syncPlot->horRangeLine1->addOffset(offset);
			syncPlot->horRangeLine2->addOffset(offset);
			syncPlot->horLine->updatePosition();
		}
	}
	else
	{
		vertRangeLine1->addOffset(offset);
		vertRangeLine2->addOffset(offset);
		vertLine->updatePosition();

		for (const auto& syncPlot : ySyncPlots)
		{
			syncPlot->horRangeLine1->addOffset(offset);
			syncPlot->horRangeLine2->addOffset(offset);
			syncPlot->horLine->updatePosition();
		}
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
		emit rangeHorDeselectedSignal();

		for (const auto& syncPlot : xSyncPlots)
		{
			syncPlot->horClickCount = 0;
			syncPlot->horRangeLine1->setVisible(false);
			syncPlot->horRangeLine2->setVisible(false);
			syncPlot->horLine->setVisible(false);
			syncPlot->horLineText->setVisible(false);
			emit syncPlot->rangeHorDeselectedSignal();
			syncPlot->layer(MARKERS_LAYER_NAME)->replot();
		}
	}
	else
	{
		vertClickCount = 0;
		vertRangeLine1->setVisible(false);
		vertRangeLine2->setVisible(false);
		vertLine->setVisible(false);
		vertLineText->setVisible(false);
		emit rangeVertDeselectedSignal();

		for (const auto& syncPlot : ySyncPlots)
		{
			syncPlot->vertClickCount = 0;
			syncPlot->vertRangeLine1->setVisible(false);
			syncPlot->vertRangeLine2->setVisible(false);
			syncPlot->vertLine->setVisible(false);
			syncPlot->vertLineText->setVisible(false);
			emit syncPlot->rangeVertDeselectedSignal();
			syncPlot->layer(MARKERS_LAYER_NAME)->replot();
		}
	}

	layer(MARKERS_LAYER_NAME)->replot();
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

void MarkingPlot::forceUpdateVerticalText()
{
	double firstPoint = vertRangeLine1->getRealCoords().y();
	double secondPoint = vertRangeLine2->getRealCoords().y();

	if (firstPoint > secondPoint)
	{
		const double temp = firstPoint;
		firstPoint = secondPoint;
		secondPoint = temp;
	}

	vertLineText->setText(setupVerticalText(firstPoint, secondPoint));
	layer(MARKERS_LAYER_NAME)->replot();
}

void MarkingPlot::forceUpdateHorizontalText()
{
	double firstPoint = horRangeLine1->getRealCoords().x();
	double secondPoint = horRangeLine2->getRealCoords().x();

	if (firstPoint > secondPoint)
	{
		const double temp = firstPoint;
		firstPoint = secondPoint;
		secondPoint = temp;
	}

	horLineText->setText(setupHorizontalText(firstPoint, secondPoint));
	layer(MARKERS_LAYER_NAME)->replot();
}

void MarkingPlot::mousePressEvent(QMouseEvent* event)
{
	QCustomPlot::mousePressEvent(event);

	mouseClickPoint = event->pos();
}

void MarkingPlot::mouseReleaseEvent(QMouseEvent* event)
{
	QCustomPlot::mouseReleaseEvent(event);
	
	if (fabs(mouseClickPoint.x() - event->pos().x()) > clickAllowableOffset || 
		fabs(mouseClickPoint.y() - event->pos().y()) > clickAllowableOffset) return;

	{
		const double x = xAxis->pixelToCoord(event->pos().x());
		const QCPRange xRange = xAxis->range();

		if (xRange.lower > x || xRange.upper < x) return;

		const double y = yAxis->pixelToCoord(event->pos().y());
		const QCPRange yRange = yAxis->range();

		if (yRange.lower > y || yRange.upper < y) return;
	}

	if (event->button() == horMouseButton && event->modifiers() == horMouseModifier 
		&& markerActiveRules[EA_xAxis])
	{
		horizontalClickEvent(event);
		return;
	}

	if (event->button() == vertMouseButton && event->modifiers() == vertMouseModifier 
		&& markerActiveRules[EA_yAxis])
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
		incrementCount(horClickCount);

		for(const auto& syncPlot: xSyncPlots)
		{
			syncPlot->setHorLineCoords(syncPlot->horRangeLine1, x);
			syncPlot->incrementCount(syncPlot->horClickCount);
		}

		return;
	}

	if (horClickCount == 1)
	{
		setHorLineCoords(horRangeLine2, x);
		incrementCount(horClickCount);

		setupHorFirstMidLinePosition();
		updateHorizontalMarkers();
		horLineText->setVisible(true);
		horLine->setVisible(true);

		layer(MARKERS_LAYER_NAME)->replot();

		double firstPoint = horRangeLine1->point1->key();
		double secondPoint = horRangeLine2->point1->key();

		if (firstPoint > secondPoint)
		{
			const double temp = firstPoint;
			firstPoint = secondPoint;
			secondPoint = temp;
		}

		emit rangeHorSelectedSignal(firstPoint, secondPoint);

		for (const auto& syncPlot : xSyncPlots)
		{
			syncPlot->setHorLineCoords(syncPlot->horRangeLine2, x);
			syncPlot->incrementCount(syncPlot->horClickCount);

			syncPlot->setupHorFirstMidLinePosition();
			syncPlot->updateHorizontalMarkers();
			syncPlot->horLineText->setVisible(true);
			syncPlot->horLine->setVisible(true);

			syncPlot->layer(MARKERS_LAYER_NAME)->replot();

			emit syncPlot->rangeHorSelectedSignal(firstPoint, secondPoint);
		}

		return;
	}

	if (horClickCount == 2)
	{
		clearMarkers(EA_xAxis);
	}
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
		incrementCount(vertClickCount);

		for (const auto& syncPlot : ySyncPlots)
		{
			syncPlot->setVertLineCoords(syncPlot->vertRangeLine1, y);
			syncPlot->incrementCount(syncPlot->vertClickCount);
		}

		return;
	}

	if (vertClickCount == 1)
	{
		setVertLineCoords(vertRangeLine2, y);
		incrementCount(vertClickCount);

		setupVertFirstMidLinePosition();
		updateVerticalMarkers();
		vertLine->setVisible(true);
		vertLineText->setVisible(true);

		layer(MARKERS_LAYER_NAME)->replot();

		double firstPoint = vertRangeLine1->point1->value();
		double secondPoint = vertRangeLine2->point1->value();

		if (firstPoint > secondPoint)
		{
			const double temp = firstPoint;
			firstPoint = secondPoint;
			secondPoint = temp;
		}
		emit rangeVertSelectedSignal(firstPoint, secondPoint);

		for (const auto& syncPlot : ySyncPlots)
		{
			syncPlot->setVertLineCoords(syncPlot->vertRangeLine2, y);
			syncPlot->incrementCount(syncPlot->vertClickCount);

			syncPlot->setupVertFirstMidLinePosition();
			syncPlot->updateVerticalMarkers();
			syncPlot->vertLine->setVisible(true);
			syncPlot->vertLineText->setVisible(true);

			syncPlot->layer(MARKERS_LAYER_NAME)->replot();

			emit syncPlot->rangeVertSelectedSignal(firstPoint, secondPoint);
		}

		return;
	}

	if (vertClickCount == 2)
	{
		clearMarkers(EA_yAxis);
	}
}

QString MarkingPlot::setupHorizontalText(double start, double end)
{
	return QString::number(end - start);
}

QString MarkingPlot::setupVerticalText(double start, double end)
{
	return QString::number(end - start);
}

void MarkingPlot::initializeRangeLine(MovableInfinityLine** line, EAxis moveAxis)
{
	(*line) = new MovableInfinityLine(this);
	(*line)->setVisible(false);
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
	(*text)->setLayer(MARKERS_LAYER_NAME);
}

void MarkingPlot::initializeVertMarkerText(QCPItemText** text)
{
	(*text) = new QCPItemText(this);
	(*text)->setPositionAlignment(Qt::AlignVCenter);
	(*text)->position->setType(QCPItemPosition::ptPlotCoords);
	(*text)->setFont(QFont(font().family(), 12));
	(*text)->setColor(Qt::black);
	(*text)->setVisible(false);
	(*text)->setLayer(MARKERS_LAYER_NAME);
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
	line->setPointCoord(line->point1, x, 0);
	line->setPointCoord(line->point2, x, 1);
	line->setPointRealCoord(x, 0);
	line->setVisible(true);

	layer(MARKERS_LAYER_NAME)->replot();
}

void MarkingPlot::setVertLineCoords(MovableInfinityLine* line, double y)
{
	line->setPointCoord(line->point1, 0, y);
	line->setPointCoord(line->point2, 1, y);
	line->setPointRealCoord(0, y);
	line->setVisible(true);

	layer(MARKERS_LAYER_NAME)->replot();
}

void MarkingPlot::setupHorFirstMidLinePosition()
{
	const auto currentRange = yAxis->range();
	const double y = currentRange.upper - ((currentRange.upper - currentRange.lower) / 2.0);

	horLine->start->setCoords(horLine->start->coords().x(), y);
	horLine->end->setCoords(horLine->start->coords().x(), horLine->start->coords().y());
}

void MarkingPlot::setupVertFirstMidLinePosition()
{
	const auto currentRange = xAxis->range();
	const double x = currentRange.lower + ((currentRange.upper - currentRange.lower) / 5.0);

	vertLine->start->setCoords(x, vertLine->start->coords().y());
	vertLine->end->setCoords(x, vertLine->start->coords().y());
}

void MarkingPlot::updateHorizontalMarkers()
{
	double firstPoint = horRangeLine1->getRealCoords().x();
	double secondPoint = horRangeLine2->getRealCoords().x();

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
	
	emit updateHorMarkersSignal(firstPoint, secondPoint);
}

void MarkingPlot::updateVerticalMarkers()
{
	double firstPoint = vertRangeLine1->getRealCoords().y();
	double secondPoint = vertRangeLine2->getRealCoords().y();

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

	emit updateVertMarkersSignal(firstPoint, secondPoint);
}

void MarkingPlot::addMarkerSyncPlot(EAxis syncAxis, MarkingPlot* plot)
{
	if (!plot) return;

	if(syncAxis == EA_xAxis)
	{
		plot->horRangeLine1->addSyncLine(horRangeLine1);
		plot->horRangeLine2->addSyncLine(horRangeLine2);
		horRangeLine1->addSyncLine(plot->horRangeLine1);
		horRangeLine2->addSyncLine(plot->horRangeLine2);

		xSyncPlots.append(plot);
	}
	else
	{
		plot->vertRangeLine1->addSyncLine(vertRangeLine1);
		plot->vertRangeLine2->addSyncLine(vertRangeLine2);
		vertRangeLine1->addSyncLine(plot->vertRangeLine1);
		vertRangeLine2->addSyncLine(plot->vertRangeLine2);

		ySyncPlots.append(plot);
	}
}

void MarkingPlot::removeMarkerSyncPlot(EAxis syncAxis, MarkingPlot* plot)
{
	if (!plot) return;

	if (syncAxis == EA_xAxis)
	{
		plot->horRangeLine1->removeSyncLine(horRangeLine1);
		plot->horRangeLine2->removeSyncLine(horRangeLine2);
		horRangeLine1->removeSyncLine(plot->horRangeLine1);
		horRangeLine2->removeSyncLine(plot->horRangeLine2);

		xSyncPlots.removeOne(plot);
	}
	else
	{
		plot->vertRangeLine1->removeSyncLine(vertRangeLine1);
		plot->vertRangeLine2->removeSyncLine(vertRangeLine2);
		vertRangeLine1->removeSyncLine(plot->vertRangeLine1);
		vertRangeLine2->removeSyncLine(plot->vertRangeLine2);

		ySyncPlots.removeOne(plot);
	}
}
