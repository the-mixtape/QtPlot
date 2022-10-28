#pragma once
#include "QtPlotGlobal.h"
#include "QCustomPlot/QCustomPlot.h"
#include "Library/QtPlotEnumLibrary.h"


class QTPLOT_EXPORT ZoomClampedPlot : public QCustomPlot
{
	Q_OBJECT

public:
	explicit ZoomClampedPlot(QWidget* parent = nullptr);
	virtual ~ZoomClampedPlot();

	inline void setAxisClamp(EAxis clampAxis, bool clamp) { clampAxisRules[clampAxis] = clamp; };
	inline void setAxisClampRange(EAxis clampAxis, const QCPRange& clampRange) { limitRangeRules[clampAxis] = clampRange; };
	inline void setSyncRule(EAxis axis, ESyncRule rule) { syncRules[axis] = rule; }

	inline bool		 getAxisClamp(EAxis axis)  { return clampAxisRules[axis]; }
	inline QCPRange  getLimitRange(EAxis axis) { return limitRangeRules[axis]; }
	inline ESyncRule getSyncRule(EAxis axis)   { return syncRules[axis]; }

private:
	void limitAxisRange(EAxis axis, QCPAxis* axisObject, const QCPRange& newRange);

	void privateSetRange(QCPAxis* axis, const QCPRange& range);
	void privateSetRange(QCPAxis* axis, EAxis axisE, const ZoomClampedPlot* plot, const QCPRange& range);

protected slots:
	virtual void xAxisChanged(const QCPRange& newRange);
	virtual void yAxisChanged(const QCPRange& newRange);

	virtual void setRangeX(const QCPRange& range);
	virtual void setRangeY(const QCPRange& range);
	
private:
	QMap<EAxis, bool>		clampAxisRules;
	QMap<EAxis, QCPRange>	limitRangeRules;
	QMap<EAxis, ESyncRule>	syncRules;
	
};

