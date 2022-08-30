#include "ZoomClampedPlot.h"

ZoomClampedPlot::ZoomClampedPlot(QWidget* parent)
	: QCustomPlot(parent)
{
    clampAxisRules[EA_xAxis] = false;
    clampAxisRules[EA_yAxis] = false;

	connect(xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
	connect(yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
}

ZoomClampedPlot::~ZoomClampedPlot()
{
}

void ZoomClampedPlot::setAxisClamp(EAxis clampAxis, bool clamp)
{
    clampAxisRules[clampAxis] = clamp;
}

void ZoomClampedPlot::setAxisClampRange(EAxis clampAxis, const QCPRange& clampRange)
{
    limitRangeRules[clampAxis] = clampRange;
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
