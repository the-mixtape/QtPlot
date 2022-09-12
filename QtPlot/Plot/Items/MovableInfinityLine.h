#pragma once

#include "Library/QtPlotEnumLibrary.h"
#include "QCustomPlot/QCustomPlot.h"


//forward declaration
class MovableItemLine;


class MovableInfinityLine : public QCPItemStraightLine
{
	Q_OBJECT

public:
	explicit MovableInfinityLine(QCustomPlot* parentPlot);
	~MovableInfinityLine() override;

	void setPoint1Coord(double x, double y);
	void setPoint2Coord(double x, double y);

	void addOffset(int inOffset);
	void setPen(ELineState inState, QPen pen);
	void setMoveAxis(EAxis inAxis);
	inline void setMidLine(MovableItemLine* line) { midLine = line; }
	
	inline void setMovable(bool active)
	{
		bIsMovable = active;
		if (active == false) setIsDrag(active);
	}

protected:
	void mousePressEvent(QMouseEvent* event, const QVariant& details) override;
	virtual void setIsDrag(bool drag);
	
	
private:
	void xMoveAxis(QMouseEvent* event);
	void yMoveAxis(QMouseEvent* event);
	void checkHovered(QMouseEvent* event);

	void setState(ELineState state);

signals:
	void updatePosition();

private slots:
	void mouseRelease(QMouseEvent* event);
	void mouseMove(QMouseEvent* event);
	void axisXChanged(const QCPRange& range);
	void axisYChanged(const QCPRange& range);

private:
	MovableItemLine* midLine;
	QCP::Interactions currentInteractions;
	QPointF realCoords;

	ELineState state = ELS_Idle;
	QMap<ELineState, QPen> pens;

	int offset;
	EAxis axis;
	bool bIsDrag	= false;
	bool bIsMovable = false;
};

