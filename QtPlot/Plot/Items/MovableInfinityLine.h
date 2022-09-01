#pragma once

#include "Library/EnumLibrary.h"
#include "QCustomPlot/QCustomPlot.h"


//forward declaration
class MovableItemLine;


class MovableInfinityLine : public QCPItemStraightLine
{
	Q_OBJECT

public:
	explicit MovableInfinityLine(QCustomPlot* parentPlot);
	~MovableInfinityLine() override;

	void setPen(ELineState inState, QPen pen);
	inline void setMoveAxis(EAxis inAxis) { axis = inAxis; }
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

private:
	MovableItemLine* midLine;
	QCP::Interactions currentInteractions;

	ELineState state = idle;
	QMap<ELineState, QPen> pens;

	EAxis axis;
	bool bIsDrag	= false;
	bool bIsMovable = false;
};

