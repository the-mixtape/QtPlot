#include "WaterfallContent.h"

#include "ColorMap/WfColorMap.h"
#include "WaterfallLayer.h"
#include "Library/QtPlotMathLibrary.h"
#include "Plot/QtPlot.h"


WaterfallContent::WaterfallContent(QCustomPlot* parent)
	: QCPItemPixmap(parent),
	waterfallLayer(nullptr),
	appendSide(EAS_Top),
	appendHeight(1)
{
	parentQtPlot = reinterpret_cast<QtPlot*>(parent);
	readWriteLock = new QReadWriteLock(QReadWriteLock::Recursive);
	readWritePixmap = new QReadWriteLock();
	setScaled(true, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

WaterfallContent::~WaterfallContent()
{
	bool success = disconnect();

	readWriteLock->lockForWrite();

	delete waterfallLayer;
	waterfallLayer = nullptr;

	readWriteLock->unlock();

	delete readWriteLock;
	readWriteLock = nullptr;

	readWritePixmap->unlock();

	delete readWritePixmap;
	readWritePixmap = nullptr;
}	

void WaterfallContent::setColorMap(WfColorMap* inColorMap)
{
	if (inColorMap == nullptr) return;

	readWriteLock->lockForWrite();

	if (waterfallLayer != nullptr)
	{
		delete waterfallLayer->colorMap;
		waterfallLayer->colorMap = inColorMap;
	}

	readWriteLock->unlock();
}

void WaterfallContent::setAppendSide(EAppendSide side)
{
	readWriteLock->lockForWrite();

	appendSide = side;

	readWriteLock->unlock();
}

void WaterfallContent::updatePixmap()
{
	readWriteLock->lockForRead();
	setPixmap(QPixmap::fromImage(*waterfallLayer->image));
	readWriteLock->unlock();
}

void WaterfallContent::update()
{
	parentPlot()->layer(WATERFALL_LAYER_NAME)->replot();
}

void WaterfallContent::setResolution(int width, int height)
{
	readWriteLock->lockForRead();
	if (waterfallLayer->image->width() == width
		&& waterfallLayer->image->height() == height)
	{
		readWriteLock->unlock();
		return;
	}
	readWriteLock->unlock();

	readWriteLock->lockForWrite();

	delete waterfallLayer->image;
	waterfallLayer->image = new QImage(width, height, waterfallLayer->format);
	waterfallLayer->image->fill(waterfallLayer->fillColor);
	update();

	readWriteLock->unlock();
}

void WaterfallContent::setWidth(int width)
{
	if (waterfallLayer->image->width() == width) return;

	const int height = waterfallLayer->image->height();

	delete waterfallLayer->image;
	waterfallLayer->image = new QImage(width, height, waterfallLayer->format);
	waterfallLayer->image->fill(waterfallLayer->fillColor);
	update();
}

void WaterfallContent::setHeight(int height)
{
	if (waterfallLayer->image->height() == height) return;

	readWriteLock->lockForWrite();

	const int width = waterfallLayer->image->width();

	delete waterfallLayer->image;
	waterfallLayer->image = new QImage(width, height, waterfallLayer->format);
	waterfallLayer->image->fill(waterfallLayer->fillColor);

	readWriteLock->unlock();

	update();
}

void WaterfallContent::setFillColor(const QColor& fillColor)
{
	readWriteLock->lockForWrite();
	
	waterfallLayer->fillColor = fillColor;
	waterfallLayer->image->fill(fillColor);
	setPixmap(QPixmap::fromImage(*waterfallLayer->image));

	readWriteLock->unlock();

	update();
}

void WaterfallContent::setInterval(int minval, int maxval)
{
	readWriteLock->lockForRead();
	{
		if(equals(minval, waterfallLayer->range.minValue()) 
			&& equals(maxval, waterfallLayer->range.maxValue()))
		{
			readWriteLock->unlock();
			return;
		}
	}
	readWriteLock->unlock();

	readWriteLock->lockForWrite();
	{
		if (waterfallLayer->range.isValid())
		{
			const auto currentInterval = waterfallLayer->range;
			waterfallLayer->range = QtInterval(minval, maxval);

			for (int h = 0; h < waterfallLayer->image->height(); h++)
			{
				auto line = reinterpret_cast<QRgb*>(waterfallLayer->image->scanLine(h));

				for (int w = 0; w < waterfallLayer->image->width(); w++)
				{
					double oldValue = waterfallLayer->colorMap->RGB2Double(currentInterval, *line);
					if(equals(oldValue, 0.0))
					{
						*line++;
						continue;
					}
					oldValue = currentInterval.minValue() + oldValue * currentInterval.width();

					*line++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, oldValue);
				}
			}
		}
	}
	readWriteLock->unlock();

	{
		readWriteLock->lockForRead();
		setPixmap(QPixmap::fromImage(*waterfallLayer->image));
		readWriteLock->unlock();

		update();
	}
}

QtInterval WaterfallContent::getInterval() const
{
	readWriteLock->lockForRead();
	const QtInterval interval = waterfallLayer->range;
	readWriteLock->unlock();

	return interval;
}

void WaterfallContent::setPositionX(int minx, int maxx)
{
	const int maxy = topLeft->coords().y();
	const int miny = bottomRight->coords().y();

	topLeft->setCoords(minx, maxy);
	bottomRight->setCoords(maxx, miny);

	update();
}

void WaterfallContent::setPositionY(int miny, int maxy)
{
	const int maxx = topLeft->coords().x();
	const int minx = bottomRight->coords().x();

	topLeft->setCoords(minx, maxy);
	bottomRight->setCoords(maxx, miny);

	update();
}

void WaterfallContent::setAppendHeight(int h)
{
	if (h == 0) return;

	readWriteLock->lockForRead();

	appendHeight = h;

	readWriteLock->unlock();
}

void WaterfallContent::clear()
{
	readWriteLock->lockForRead();
	
	waterfallLayer->image->fill(waterfallLayer->fillColor);
	setPixmap(QPixmap::fromImage(*waterfallLayer->image));

	readWriteLock->unlock();

	update();
}

void WaterfallContent::append(double* data, int size, bool needUpdatePixmap/* = true*/)
{
	readWriteLock->lockForRead();

	switch (appendSide)
	{
		case EAS_Top:
		{
			appendT(data, size, appendHeight);
			break;
		}

		case EAS_Bottom:
		{
			appendB(data, size, appendHeight);
			break;
		}

		case EAS_Left:
		{
			appendL(data, size, appendHeight);
			break;
		}

		case EAS_Right:
		{
			appendR(data, size, appendHeight);
			break;
		}
	}

	if (needUpdatePixmap)
	{
		readWritePixmap->lockForWrite();
		setPixmap(QPixmap::fromImage(*waterfallLayer->image));
		readWritePixmap->unlock();
	}

	readWriteLock->unlock();
}

bool WaterfallContent::createLayer(qint32 inWidth, qint32 inHeight, qreal minx, qreal miny, qreal maxx, qreal maxy,
                                qreal minval, qreal maxval, QImage::Format fm, QColor fil)
{
	delete waterfallLayer;

	readWriteLock->lockForWrite();

	waterfallLayer = new WaterfallLayer();
	waterfallLayer->image = new QImage(inWidth, inHeight, fm);
	waterfallLayer->format = fm;
	waterfallLayer->image->fill(fil);
	waterfallLayer->fillColor = fil;
	
	setPixmap(QPixmap::fromImage(*waterfallLayer->image));

	waterfallLayer->range = QtInterval(minval, maxval);

	topLeft->setCoords(minx, maxy);
	bottomRight->setCoords(maxx, miny);

	readWriteLock->unlock();

	return true;
}

void WaterfallContent::appendT(double* data, int w, int h)
{
	if (waterfallLayer->image->width() > w) return;

	int y = 0;
	uchar* imageData = waterfallLayer->image->bits();
	memmove(imageData + waterfallLayer->image->bytesPerLine() * h, 
		imageData, 
		waterfallLayer->image->sizeInBytes() - waterfallLayer->image->bytesPerLine() * h);

	while (y < h)
	{
		QRgb* line = reinterpret_cast<QRgb*>(waterfallLayer->image->scanLine(y));
		for (int x = 0; x < w; x++)
		{
			*line++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[x]);
		}
		y++;
	}
}

void WaterfallContent::appendB(double* data, int w, int h)
{
	if (waterfallLayer->image->width() > w) return;

	int	y = h;
	uchar* imageData = waterfallLayer->image->bits();
	memmove(imageData, 
		imageData + waterfallLayer->image->bytesPerLine() * h, 
		waterfallLayer->image->sizeInBytes() - waterfallLayer->image->bytesPerLine() * h);

	while (y >= 1) 
	{
		QRgb* line = reinterpret_cast<QRgb*>(waterfallLayer->image->scanLine(waterfallLayer->image->height() - y));
		for (int x = 0; x < w; x++)
			*line++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[x]);
		y--;
	}
}

void WaterfallContent::appendL(double* data, int w, int h)
{
	if (waterfallLayer->image->height() > w) return;

	const qint32 bpp = waterfallLayer->image->depth() / 8;
	qint32 offset = 0;

	for (int i = 0; i < w; i++)
	{
		uchar* line = waterfallLayer->image->scanLine(i);
		memmove(line + h * bpp, line, waterfallLayer->image->bytesPerLine() - h * bpp);
		QRgb* lina = reinterpret_cast<QRgb*>(line);
		for (int x = offset; x <= offset + h; x++)
			*lina++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[i]);
		offset += h;
	}
}

void WaterfallContent::appendR(double* data, int w, int h)
{
	if (waterfallLayer->image->height() > w) return;

	const qint32 bpp = waterfallLayer->image->depth() / 8;
	qint32 offset = 0;

	for (int i = 0; i < w; i++)
	{
		uchar* line = waterfallLayer->image->scanLine(i);
		memmove(line, line + h * bpp, waterfallLayer->image->bytesPerLine() - h * bpp);
		QRgb* lina = reinterpret_cast<QRgb*>(line + waterfallLayer->image->bytesPerLine() - h * bpp);
		for (int x = offset; x <= offset + h; x++)
			*lina++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[i]);
		offset += h;
	}
}

void WaterfallContent::draw(QCPPainter* painter)
{
	bool flipHorz = false;
	bool flipVert = false;
	QRect rect = getFinalRect(&flipHorz, &flipVert);
	int clipPad = mainPen().style() == Qt::NoPen ? 0 : qCeil(mainPen().widthF());
	QRect boundingRect = rect.adjusted(-clipPad, -clipPad, clipPad, clipPad);
	if (boundingRect.intersects(clipRect()))
	{
		setupScaledPixmap(rect);
		painter->drawPixmap(clipRect().topLeft(), mScaledPixmap);
		QPen pen = mainPen();
		if (pen.style() != Qt::NoPen)
		{
			painter->setPen(pen);
			painter->setBrush(Qt::NoBrush);
			painter->drawRect(clipRect());
		}
	}
}

void WaterfallContent::setupScaledPixmap(QRect finalRect)
{
	if (mPixmap.isNull())
		return;

	if (mScaled)
	{
#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
		double devicePixelRatio = mPixmap.devicePixelRatio();
#else
		double devicePixelRatio = 1.0;
#endif
		if (finalRect.isNull())
			return;

		const auto xRange = parentQtPlot->xAxis->range();
		const auto yRange = parentQtPlot->yAxis->range();

		if (mScaledPixmapInvalidated || lastFinalRect.size() != finalRect.size() || xLastRange != xRange || yLastRange != yRange)
		{
			xLastRange = xRange;
			yLastRange = yRange;
			
			const QCPRange xLimitRange = QCPRange(topLeft->coords().x(), bottomRight->coords().x());
			const QCPRange yLimitRange = QCPRange(bottomRight->coords().y(), topLeft->coords().y());

			const double xDelta = xLimitRange.upper - xLimitRange.lower;
			const double yDelta = yLimitRange.upper - yLimitRange.lower;

			const double xMultiply = (xRange.upper - xRange.lower) / xDelta;
			const double yMultiply = (yRange.upper - yRange.lower) / yDelta;

			double width = mPixmap.width() * xMultiply;
			if (width < 1.0) width = 1.0;

			double height = mPixmap.height() * yMultiply;
			if (height < 1.0) height = 1.0;

			const double xOffset = (xRange.lower - xLimitRange.lower) / xDelta * mPixmap.width();
			const double yOffset = (yLimitRange.upper - yRange.upper) / yDelta * mPixmap.height();

			const QRect baseRect = clipRect();
			const QRect copyRect(xOffset, yOffset, width, height);

			readWritePixmap->lockForRead();
			mScaledPixmap = mPixmap.copy(copyRect);
			readWritePixmap->unlock();

			mScaledPixmap = mScaledPixmap.scaled(baseRect.size() * devicePixelRatio, mAspectRatioMode, mTransformationMode);

#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
			mScaledPixmap.setDevicePixelRatio(devicePixelRatio);
#endif
		}

		lastFinalRect = finalRect;
	}
	else if (!mScaledPixmap.isNull())
		mScaledPixmap = QPixmap();
	mScaledPixmapInvalidated = false;
}
