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

	inline void setMoveAxis(EAxis inAxis) { axis = inAxis; }
	inline void setMidLine(MovableItemLine* line) { midLine = line; }

protected:
	void mousePressEvent(QMouseEvent* event, const QVariant& details) override;
	virtual void setIsDrag(bool drag);

private:
	void xMoveAxis(QMouseEvent* event);
	void yMoveAxis(QMouseEvent* event);

signals:
	void updatePosition();

private slots:
	void mouseRelease(QMouseEvent* event);
	void mouseMove(QMouseEvent* event);

private:
	MovableItemLine* midLine;
	QCP::Interactions currentInteractions;
	EAxis axis;
	bool bIsDrag = false;
};

