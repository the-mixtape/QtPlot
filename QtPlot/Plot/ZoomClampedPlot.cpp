#include "ZoomClampedPlot.h"

ZoomClampedPlot::ZoomClampedPlot(QWidget* parent)
	: SettingingPlot(parent)
{
    clampAxisRules[EA_xAxis] = false;
    clampAxisRules[EA_yAxis] = false;

    syncRules[EA_xAxis] = ESR_Range;
    syncRules[EA_yAxis] = ESR_Range;

	connect(xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
	connect(yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
}

ZoomClampedPlot::~ZoomClampedPlot()
{
}

void ZoomClampedPlot::limitAxisRange(EAxis axis, QCPAxis* axisObject, const QCPRange& newRange)
{
    if (clampAxisRules[axis] == false) return;

	const double lowerBound = limitRangeRules[axis].lower;
    const double upperBound = limitRangeRules[axis].upper;

    // code assumes upperBound > lowerBound
    QCPRange fixedRange(newRange);
    if (fixedRange.lower < lowerBound)
    {
        fixedRange.lower = lowerBound;
        fixedRange.upper = lowerBound + newRange.size();
        if (fixedRange.upper > upperBound || qFuzzyCompare(newRange.size(), upperBound - lowerBound))
            fixedRange.upper = upperBound;
        axisObject->setRange(fixedRange); // adapt this line to use your plot/axis
    }
    else if (fixedRange.upper > upperBound)
    {
        fixedRange.upper = upperBound;
        fixedRange.lower = upperBound - newRange.size();
        if (fixedRange.lower < lowerBound || qFuzzyCompare(newRange.size(), upperBound - lowerBound))
            fixedRange.lower = lowerBound;
        axisObject->setRange(fixedRange); // adapt this line to use your plot/axis
    }
}

void ZoomClampedPlot::privateSetRange(QCPAxis* axis, const QCPRange& range)
{
    if(axis->range() != range)
    {
        axis->setRange(range);
        layer("axes")->replot();
    }
}

void ZoomClampedPlot::privateSetRange(QCPAxis* axis, EAxis axisE, const ZoomClampedPlot* plot, const QCPRange& range)
{
	const QCPRange plotLimit = plot->limitRangeRules[axisE];
    const QCPRange limitRange = limitRangeRules[axisE];

    const double delta = plotLimit.upper - plotLimit.lower;
	const double upperPercentage = (plotLimit.upper - range.upper) / delta;
	const double lowerPercentage = (plotLimit.lower - range.lower) / delta;

    const double currentDelta = limitRange.upper - limitRange.lower;
    const QCPRange newRange = QCPRange(
        limitRange.lower - (currentDelta * lowerPercentage),
        limitRange.upper - (currentDelta * upperPercentage));

    privateSetRange(axis, newRange);
}

void ZoomClampedPlot::xAxisChanged(const QCPRange& newRange)
{
	const auto axisObject = qobject_cast<QCPAxis*>(QObject::sender());
	limitAxisRange(EA_xAxis, axisObject, newRange);
}

void ZoomClampedPlot::yAxisChanged(const QCPRange& newRange)
{
	const auto axisObject = qobject_cast<QCPAxis*>(QObject::sender());
	limitAxisRange(EA_yAxis, axisObject, newRange);
}

void ZoomClampedPlot::setRangeX(const QCPRange& range)
{
    if (syncRules[EA_xAxis] == ESR_Percentage)
    {
        const auto axisObject = qobject_cast<QCPAxis*>(QObject::sender());
        const auto plot = qobject_cast<ZoomClampedPlot*>(axisObject->parentPlot());

        privateSetRange(xAxis, EA_xAxis, plot, range);
    }
    else if(syncRules[EA_xAxis] == ESR_Range)
    {
        privateSetRange(xAxis, range);
    }
}

void ZoomClampedPlot::setRangeY(const QCPRange& range)
{
    if (syncRules[EA_yAxis] == ESR_Percentage)
    {
        const auto axisObject = qobject_cast<QCPAxis*>(QObject::sender());
        const auto plot = qobject_cast<ZoomClampedPlot*>(axisObject->parentPlot());

        privateSetRange(yAxis, EA_yAxis, plot, range);
    }
    else if (syncRules[EA_yAxis] == ESR_Range)
    {
        privateSetRange(yAxis, range);
    }
}
