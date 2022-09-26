#include "InfoPlot.h"

InfoPlot::InfoPlot(QWidget* parent)
	:MarkingPlot(parent)
{
	graphMarker = addGraph();
    graphMarker->setLayer(MARKERS_LAYER_NAME);
    graphMarker->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,
        QColor(255, 255, 255), QColor(255, 255, 255, 50), 8));
	graphMarker->setVisible(false);
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

void InfoPlot::clearInfoMarker() const
{
    graphMarker->clearData();
    layer(MARKERS_LAYER_NAME)->replot();
}

void InfoPlot::setMarkerScatterStyle(QCPScatterStyle style) const
{
    graphMarker->setScatterStyle(style);
}

void InfoPlot::setShowInfoRule(EShowInfoRule rule)
{
    if (showInfoRule == rule) return;

    showInfoRule = rule;
    graphMarker->setVisible(showInfoRule == ESIR_Attach);
}

void InfoPlot::setInfoGraphIndex(int index)
{
    if (infoGraphIndex == index || index < 0 || index > graphCount()) return;

    infoGraphIndex = index;
}

QString InfoPlot::createTooltipText(double x, double y)
{
    return QString::number(x) + ":" + QString::number(y);
}

QString InfoPlot::updateTooltip(double x, double y)
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
                {
                    graphMarker->clearData();
                    graphMarker->addData(point->key, point->value);
                    layer(MARKERS_LAYER_NAME)->replot();
                }

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

