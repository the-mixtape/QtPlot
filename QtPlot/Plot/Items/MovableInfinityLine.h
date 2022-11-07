#pragma once

#include "Library/QtPlotEnumLibrary.h"
#include "BaseMarker.h"


//forward declaration
class MovableItemLine;
class MarkingPlot;


class QTPLOT_EXPORT MovableInfinityLine : public BaseMarker
{
	Q_OBJECT

public:
	explicit MovableInfinityLine(QCustomPlot* parentPlot);
	~MovableInfinityLine() override;

	void setPointCoord(QCPItemPosition* point, double x, double y);
	void setPointRealCoord(double x, double y);

	void addOffset(int inOffset);
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
	
private:
	void xMoveAxis(QMouseEvent* event) override;
	void yMoveAxis(QMouseEvent* event) override;

signals:
	void updatePosition();

private slots:
	bool mouseMove(QMouseEvent* event) override;

	void axisXChanged(const QCPRange& range);
	void axisYChanged(const QCPRange& range);

private:
	MovableItemLine* midLine;
	QCP::Interactions currentInteractions;
	QPointF realCoords;

	int offset;
	bool bIsMovable = false;

public:
	void addSyncLine(MovableInfinityLine* line);
	void removeSyncLine(MovableInfinityLine* line);

private:
	QList<MovableInfinityLine*> syncLines;

};

