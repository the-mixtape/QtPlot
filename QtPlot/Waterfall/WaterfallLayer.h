#pragma once

#include <QImage>
#include <QObject>



class WaterfallLayer : public QObject
{
	Q_OBJECT

public:
	WaterfallLayer();

public:
	QImage			image;
	QImage::Format	format;
	QColor			fillColor;
	qreal			minX;
	qreal			minY;
	qreal			maxX;
	qreal			maxY;
	qreal			opacity;

};
