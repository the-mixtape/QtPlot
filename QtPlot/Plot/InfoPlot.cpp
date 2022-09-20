#include "InfoPlot.h"

InfoPlot::InfoPlot(QWidget* parent)
	:MarkingPlot(parent)
{
}

InfoPlot::~InfoPlot()
{
}

void InfoPlot::enableTooltip() const
{
    connect(this, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMovePlot(QMouseEvent*)));
}

void InfoPlot::disableTooltip() const
{
    disconnect(this, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMovePlot(QMouseEvent*)));
}

QString InfoPlot::createTooltipText(double x, double y) const
{
    return QString::number(x) + ":" + QString::number(y);
}

QString InfoPlot::updateTooltip(double x, double y) const
{
    if(showInfoRule == ESIR_None)
    {
        return createTooltipText(x, y);
    }

	if(showInfoRule == ESIR_Attach)
    {
        if (infoGraphIndex < 0 && infoGraphIndex > graphCount()) return "";
        
        const int size = graph(infoGraphIndex)->data()->size();

        for(int index = 0; index < size; index++)
        {
	        const auto point = graph(infoGraphIndex)->data()->at(index);
			if(fabs(point->key - x) < 0.5)
            {
                return createTooltipText(point->key, point->value);
            }
        }
    }

    return "";
}

void InfoPlot::mouseMovePlot(QMouseEvent* event)
{
	const double x = xAxis->pixelToCoord(event->pos().x());
    const double y = yAxis->pixelToCoord(event->pos().y());

    QToolTip::showText(
        event->globalPos(),
        updateTooltip(x, y),
        this);
}

