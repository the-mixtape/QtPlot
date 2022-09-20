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

	inline void setShowInfoRule(EShowInfoRule rule) { showInfoRule = rule; }
	inline void setInfoGraphIndex(int index) { infoGraphIndex = index; }

	virtual QString createTooltipText(double x, double y) const;

private:
	QString updateTooltip(double x, double y) const;

private slots:
	void mouseMovePlot(QMouseEvent* event);

private:
	EShowInfoRule	showInfoRule = ESIR_None;
	int				infoGraphIndex = 0;

};