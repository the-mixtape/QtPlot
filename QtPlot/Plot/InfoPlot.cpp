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

QString InfoPlot::updateTooltip(double x, double y) const
{
    if(showInfoRule == ESIR_None)
    {
        const QString xString = QString::number(static_cast<int>(x));
        const QString yString = QString::number(static_cast<int>(y));

        return xString + ":" + yString;
    }

	if(showInfoRule == ESIR_Attach)
    {
        if (infoGraphIndex < 0 && infoGraphIndex > graphCount()) return "";

        QString str = "";
        const int size = graph(infoGraphIndex)->data()->size();

        for(int index = 0; index < size; index++)
        {
	        const auto point = graph(infoGraphIndex)->data()->at(index);
			if(fabs(point->key - x) < 0.5)
            {
                str = QString::number(point->key) + ":" + QString::number(point->value);
                break;
            }
        }

        return str;
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

