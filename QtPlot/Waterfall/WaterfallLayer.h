#pragma once

#include <QImage>
#include <QObject>

#include "Interval.h"


class WfColorMap;


class QTPLOT_EXPORT WaterfallLayer : public QObject
{
	Q_OBJECT

public:
	WaterfallLayer();
	~WaterfallLayer();

public:
	QImage*			image;
	QImage::Format	format;
	QColor			fillColor;
	QtInterval		range;
	WfColorMap*		colorMap;

};
