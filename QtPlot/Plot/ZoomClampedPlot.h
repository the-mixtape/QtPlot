#pragma once
#include "QtPlotGlobal.h"
#include "QCustomPlot/QCustomPlot.h"
#include "Library/EnumLibrary.h"


class QTPLOT_EXPORT ZoomClampedPlot : public QCustomPlot
{
	Q_OBJECT

public:
	explicit ZoomClampedPlot(QWidget* parent = nullptr);
	virtual ~ZoomClampedPlot();

	void setAxisClamp(EAxis clampAxis, bool clamp);
	void setAxisClampRange(EAxis clampAxis, const QCPRange& clampRange);

private:
	void limitAxisRange(EAxis axis, QCPAxis* axisObject, const QCPRange& newRange);

protected slots:
	virtual void xAxisChanged(const QCPRange& newRange);
	virtual void yAxisChanged(const QCPRange& newRange);
	
private:
	QMap<EAxis, bool>		clampAxisRules;
	QMap<EAxis, QCPRange>	limitRangeRules;
	
};

