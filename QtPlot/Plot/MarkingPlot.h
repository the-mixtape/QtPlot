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

	void clearMarkers(EAxis axis);

	//=======================================
	//set Style Functions
	void setMarkersPen(EAxis axis, ELineState state, QPen pen);
	void setMidMarkerPen(EAxis axis, ELineState state, QPen pen);
	void setTextFont(EAxis axis, QFont font);
	void setTextColor(EAxis axis, QColor color);
	//=======================================

	void setSetupHorLineTextFunc(QString(*func) (double start, double end) = nullptr);
	void setSetupVertLineTextFunc(QString(*func) (double start, double end) = nullptr);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	virtual void horizontalClickEvent(QMouseEvent* event);
	virtual void verticalClickEvent(QMouseEvent* event);

private:
	void initializeRangeLine(MovableInfinityLine** line, EAxis moveAxis);
	void initializeLine(MovableItemLine** line, MovableInfinityLine* fMarker, MovableInfinityLine* sMarker, EAxis moveAxis, QCPItemText* text);
	void initializeHorMarkerText(QCPItemText** text);
	void initializeVertMarkerText(QCPItemText** text);

	void incrementCount(short& count);

	void setHorLineCoords(QCPItemStraightLine* line, double x);
	void setVertLineCoords(QCPItemStraightLine* line, double y);

	QString setupHorizontalText(double start, double end);
	QString setupVerticalText(double start, double end);

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

	QString(*setupHorLineFunc) (double start, double end) = nullptr;
	QString(*setupVertLineFunc) (double start, double end) = nullptr;

	QMap<EAxis, bool> markerAttachRules;
	QMap<EAxis, bool> markerActiveRules;
	QPoint mouseClickPoint;

};

inline QString MarkingPlot::setupHorizontalText(double start, double end)
{
	if (setupHorLineFunc == nullptr) return QString::number(end - start);

	return setupHorLineFunc(start, end);
}

inline QString MarkingPlot::setupVerticalText(double start, double end)
{
	if (setupVertLineFunc == nullptr) return QString::number(end - start);

	return setupVertLineFunc(start, end);
}
