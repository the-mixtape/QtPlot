#pragma once

#include "MarkingPlot.h"

class QTPLOT_EXPORT InfoPlot : public MarkingPlot
{
	Q_OBJECT

public:
	InfoPlot(QWidget* parent = nullptr);
	virtual ~InfoPlot();

	void enableTooltip() const;
	void disableTooltip() const;

	void clearInfoMarker() const;

	void setMarkerScatterStyle(QCPScatterStyle style) const;
	void setShowInfoRule(EShowInfoRule rule);
	void setInfoGraphIndex(int index);

	virtual QString createTooltipText(double x, double y);

private:
	QString updateTooltip(double x, double y);

private slots:
	void mouseMovePlot(QMouseEvent* event);

private:
	EShowInfoRule	showInfoRule = ESIR_None;
	int				infoGraphIndex = 1;

	QCPGraph*		graphMarker;

};