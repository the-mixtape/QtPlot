#pragma once

#include "ZoomClampedPlot.h"


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
	{
		vertMouseButton = button;
	}

	inline void setAttachRule(EAxis axis, bool bNeedAttach)
	{
		markerAttachRules[axis] = bNeedAttach;
	}

	void setMarkerAxisAttach(EAxis axis, bool isNeedAttach);
	void setMarkerAxisActive(EAxis axis, bool isActive);

	void setPressLimitTime(qint64 msec);

	void clearMarkers(EAxis axis);

	//=======================================
	//set Style Functions
	void setHorMarkersPen(QPen pen);
	void setHorMidMarkerPen(QPen pen);
	void setHorTextFont(QFont font);
	void setHorTextColor(QColor color);

	void setVertMarkersPen(QPen pen);
	void setVertMidMarkerPen(QPen pen);
	void setVertTextFont(QFont font);
	void setVertTextColor(QColor color);
	//=======================================

	void setSetupHorLineTextFunc(QString(*func) (double start, double end) = nullptr);
	void setSetupVertLineTextFunc(QString(*func) (double start, double end) = nullptr);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	virtual void horizontalClickEvent(QMouseEvent* event);
	virtual void verticalClickEvent(QMouseEvent* event);

private:
	void initializeRangeLine(QCPItemStraightLine** line);
	void initializeLine(QCPItemLine** line);
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

private:
	QCPItemStraightLine* horRangeLine1;
	QCPItemStraightLine* horRangeLine2;
	QCPItemLine* horLine;
	QCPItemText* horLineText;

	QCPItemStraightLine* vertRangeLine1;
	QCPItemStraightLine* vertRangeLine2;
	QCPItemLine* vertLine;
	QCPItemText* vertLineText;

	Qt::MouseButton horMouseButton = Qt::LeftButton;
	Qt::MouseButton vertMouseButton = Qt::MidButton;

	qint64 pressLimitTime = 150;

	short horClickCount = 0;
	short vertClickCount = 0;

	QString(*setupHorLineFunc) (double start, double end) = nullptr;
	QString(*setupVertLineFunc) (double start, double end) = nullptr;

	QMap<EAxis, bool> markerAttachRules;
	QMap<EAxis, bool> markerActiveRules;
	QElapsedTimer timer;

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
