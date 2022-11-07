#pragma once

#include "Library/QtPlotEnumLibrary.h"
#include "QCustomPlot/QCustomPlot.h"

class MarkingPlot;

class QTPLOT_EXPORT BaseMarker : public QCPItemStraightLine
{
	Q_OBJECT

public:
	BaseMarker(QCustomPlot* parentPlot);

	void setPen(ELineState inState, QPen pen);

	inline void setAxis(EAxis inAxis) { axis = inAxis; }
	inline EAxis getAxis() const { return axis; }

protected:
	void replot();
	void checkHovered(QMouseEvent* event);
	void setState(ELineState inState);
	void setIsDrag(bool drag);
	bool getIsDrag() const { return bIsDrag; }

	virtual void xMoveAxis(QMouseEvent* event) = 0;
	virtual void yMoveAxis(QMouseEvent* event) = 0;

	void mousePressEvent(QMouseEvent* event, const QVariant& details) override;

protected slots:
	void mouseRelease(QMouseEvent* event);
	virtual void mouseMove(QMouseEvent* event);

private:
	ELineState state = ELS_Idle;
	QMap<ELineState, QPen> pens;

	EAxis axis;

	bool bIsDrag = false;

	QCP::Interactions currentInteractions;

	MarkingPlot* markingPlot = nullptr;
};

