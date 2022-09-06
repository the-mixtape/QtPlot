#pragma once

#include <qreadwritelock.h>
#include "QCustomPlot/QCustomPlot.h"

#include "Interval.h"

class QCustomPlot;
class WfColorMap;
class WaterfallLayer;
class QCPItemPixmap;


class WaterfallContent : public QCPItemPixmap
{
	Q_OBJECT

public:
	explicit WaterfallContent(QCustomPlot* parent = nullptr);
	~WaterfallContent() override;

public:
	void setColorMap(WfColorMap* inColorMap);

public slots:
	void update();

public:
	/*!
	  \brief Adds a new layer

	  New _layer is an image (2D) with colored data points
	  Color map is used to map values inside the 2D region to color value.

	  \note It is normal to use opacity level. Basic styling of underlying QwtPlot::canvas() is supported.

	  \param width Widget width (as Qt widget)
	  \param height Widget height (as Qt widget)
	  \param minx Minimum x value for the layer
	  \param maxx Maximum x value for the layer
	  \param miny Minimum y value for the layer
	  \param maxy Maximum y value for the layer
	  \param minval Minimum (expected) value of data
	  \param maxval Maximum (expected) value of data
	  \param fm Of type QImage::Format. Supported QImage::Format_ARGB32 and QImage::Format_RGB32.
	  \param fil Fill color for the layer (QColor).
	 */
	bool addLayer(qint32 width, qint32 height, qreal minx, qreal miny, qreal maxx, qreal maxy, qreal minval, qreal maxval, QImage::Format fm, QColor fil);
	/*!
  \brief Append data from top

  Data 'data' is a linear array (of doubles) of size w*h.
  
  \param data Array of double values. Size: w*h.
  \param w Width of the data block.
  \param h Height of the data block.
 */
	void appendT(double* data, int w, int h);

private:
	WaterfallLayer* waterfallLayer;
	QReadWriteLock* readWriteLock;

};

