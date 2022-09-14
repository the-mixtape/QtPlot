#pragma once

#include <qreadwritelock.h>
#include "QCustomPlot/QCustomPlot.h"

#include "Interval.h"
#include "Library/QtPlotEnumLibrary.h"

class QCustomPlot;
class WfColorMap;
class WaterfallLayer;
class QtPlot;


class WaterfallContent : public QCPItemPixmap
{
	Q_OBJECT

public:
	explicit WaterfallContent(QCustomPlot* parent = nullptr);
	~WaterfallContent() override;

public:
	void setColorMap(WfColorMap* inColorMap);
	void setAppendSide(EAppendSide side);

public slots:
	void update();

public:
	void setResolution(int width, int height);
	void setWidth(int width);
	void setHeight(int height);
	void setFillColor(const QColor& fillColor);
	void setInterval(int minval, int maxval);
	void setPositionX(int minx, int maxx);
	void setPositionY(int miny, int maxy);
	void setAppendHeight(int h);

	void clear();

	/*!
	\brief Append data

	Data 'data' is a linear array (of doubles) of size size*h.

	\param data Array of double values. Size: w*h.
	\param size Width of the data block.
	*/
	void append(double* data, int size);

	/*!
	  \brief Create layer

	  Layer is an image (2D) with colored data points
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
	bool createLayer(qint32 width, qint32 height, qreal minx, qreal miny, qreal maxx, qreal maxy, qreal minval, qreal maxval, QImage::Format fm, QColor fil);

private:
	/*!
  \brief Append data from top

  Data 'data' is a linear array (of doubles) of size w*h.
  
  \param data Array of double values. Size: w*h.
  \param w Width of the data block.
  \param h Height of the data block.
	*/
	void appendT(double* data, int w, int h);

	/*!
  \brief Append data from bottom

  Data 'data' is a linear array (of doubles) of size w*h.

  \param data Array of double values. Size: w*h.
  \param w Width of the data block.
  \param h Height of the data block.
	*/
	void appendB(double* data, int w, int h);

	/*!
  \brief Append data from left

  Data 'data' is a linear array (of doubles) of size w*h.

  \param data Array of double values. Size: w*h.
  \param w Width of the data block.
  \param h Height of the data block.
	*/
	void appendL(double* data, int w, int h);

	/*!
  \brief Append data from right

  Data 'data' is a linear array (of doubles) of size w*h.

  \param data Array of double values. Size: w*h.
  \param w Width of the data block.
  \param h Height of the data block.
	*/
	void appendR(double* data, int w, int h);

protected:
	void draw(QCPPainter* painter) override;

private:
	void setupScaledPixmap(QRect finalRect, bool flipHorz, bool flipVert);

private:
	QRect			lastFinalRect;

	WaterfallLayer* waterfallLayer;
	QReadWriteLock* readWriteLock;
	QtPlot*			parentQtPlot;
	EAppendSide		appendSide;
	qint32			appendHeight;

	QCPRange xLastRange;
	QCPRange yLastRange;

};

