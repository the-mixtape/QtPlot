#pragma once

#include "ZoomClampedPlot.h"


//Forward Declaration
class MovableItemLine;
class MovableInfinityLine;


class QTPLOT_EXPORT MarkingPlot : public ZoomClampedPlot
{
	Q_OBJECT

public:
	explicit MarkingPlot(QWidget* parent = nullptr);
	virtual ~MarkingPlot();

	inline void setHorizontalSelectButton(Qt::MouseButton button)
	{
		horMouseButton = button;
	}

	inline void setVerticalSelectButton(Qt::MouseButton button)
	{ vertMouseButton = button; }

	inline void setAttachRule(EAxis axis, bool bNeedAttach)
	{ markerAttachRules[axis] = bNeedAttach; }

	void setMarkerAxisAttach(EAxis axis, bool isNeedAttach);
	void setMarkerAxisActive(EAxis axis, bool isActive);
	void setMarkerMovableAxisActive(EAxis axis, bool isActive) const;
	void addMarkersOffset(EAxis axis, int offset) const;

	void clearMarkers(EAxis axis);

	//=======================================
	//set Style Functions
	void setMarkersPen(EAxis axis, ELineState state, QPen pen);
	void setMidMarkerPen(EAxis axis, ELineState state, QPen pen);
	void setTextFont(EAxis axis, QFont font);
	void setTextColor(EAxis axis, QColor color);
	//=======================================

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	virtual void horizontalClickEvent(QMouseEvent* event);
	virtual void verticalClickEvent(QMouseEvent* event);

	virtual QString setupHorizontalText(double start, double end);
	virtual QString setupVerticalText(double start, double end);

private:
	void initializeRangeLine(MovableInfinityLine** line, EAxis moveAxis);
	void initializeLine(MovableItemLine** line, MovableInfinityLine* fMarker, MovableInfinityLine* sMarker, EAxis moveAxis, QCPItemText* text);
	void initializeHorMarkerText(QCPItemText** text);
	void initializeVertMarkerText(QCPItemText** text);

	void incrementCount(short& count);

	void setHorLineCoords(MovableInfinityLine* line, double x);
	void setVertLineCoords(MovableInfinityLine* line, double y);

	void setupHorFirstMidLinePosition();
	void setupVertFirstMidLinePosition();

signals:
	void rangeHorSelectedSignal(double first, double second);
	void rangeVertSelectedSignal(double first, double second);

private slots:
	void updateHorizontalMarkers();
	void updateVerticalMarkers();

private:
	MovableInfinityLine* horRangeLine1;
	MovableInfinityLine* horRangeLine2;
	MovableItemLine* horLine;
	QCPItemText* horLineText;

	MovableInfinityLine* vertRangeLine1;
	MovableInfinityLine* vertRangeLine2;
	MovableItemLine* vertLine;
	QCPItemText* vertLineText;

	Qt::MouseButton horMouseButton = Qt::LeftButton;
	Qt::MouseButton vertMouseButton = Qt::MidButton;

	short horClickCount = 0;
	short vertClickCount = 0;

	QMap<EAxis, bool> markerAttachRules;
	QMap<EAxis, bool> markerActiveRules;
	QPoint mouseClickPoint;

public:
	void addMarkerSyncPlot(EAxis syncAxis, MarkingPlot* plot);
	void removeMarkerSyncPlot(EAxis syncAxis, MarkingPlot* plot);

private:
	QList<MarkingPlot*> xSyncPlots;
	QList<MarkingPlot*> ySyncPlots;

	quint8 xSyncPlotsCount;
	quint8 ySyncPlotsCount;

};

