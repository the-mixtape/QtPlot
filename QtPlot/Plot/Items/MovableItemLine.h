#pragma once

#include "QCustomPlot/QCustomPlot.h"
#include "Library/QtPlotEnumLibrary.h"


class MovableInfinityLine;


class MovableItemLine : public QCPItemLine
{
	Q_OBJECT

public:
	explicit MovableItemLine(QCustomPlot* parentPlot, MovableInfinityLine* fMarker, MovableInfinityLine* sMarker, QCPItemText* text = nullptr);
	~MovableItemLine() override;

	inline void setMoveAxis(EAxis inAxis) { axis = inAxis; }
	void updatePosition();

	void setPen(ELineState state, QPen pen);

protected:
	void mousePressEvent(QMouseEvent* event, const QVariant& details) override;
	virtual void setIsDrag(bool drag);

private:
	void xMoveAxis(QMouseEvent* event);
	void yMoveAxis(QMouseEvent* event);
	void checkHovered(QMouseEvent* event);
	void setState(ELineState state);

	void xUpdate();
	void yUpdate();

private slots:
	void mouseRelease(QMouseEvent* event);
	void mouseMove(QMouseEvent* event);

private:
	MovableInfinityLine* firstMarker;
	MovableInfinityLine* secondMarker;
	QCPItemText* attachedText;
	QCP::Interactions currentInteractions;
	ELineState state = ELS_Idle;
	QMap<ELineState, QPen> pens;
	EAxis axis;
	bool bIsDrag = false;

};

