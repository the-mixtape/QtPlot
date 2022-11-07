#pragma once

#include "Library/QtPlotEnumLibrary.h"
#include "QCustomPlot/QCustomPlot.h"


//forward declaration
class MovableItemLine;
class MarkingPlot;


class MovableInfinityLine : public QCPItemStraightLine
{
	Q_OBJECT

public:
	explicit MovableInfinityLine(QCustomPlot* parentPlot);
	~MovableInfinityLine() override;

	void setPointCoord(QCPItemPosition* point, double x, double y);
	void setPointRealCoord(double x, double y);

	void addOffset(int inOffset);
	void setPen(ELineState inState, QPen pen);
	void setMoveAxis(EAxis inAxis);
	inline void setMidLine(MovableItemLine* line) { midLine = line; }
	
	inline void setMovable(bool active)
	{
		bIsMovable = active;
		if (active == false) setIsDrag(active);
	}

	inline QPointF getRealCoords() const { return realCoords; }

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

public:
	void addSyncLine(MovableInfinityLine* line);
	void removeSyncLine(MovableInfinityLine* line);

private:
	QList<MovableInfinityLine*> syncLines;

};

