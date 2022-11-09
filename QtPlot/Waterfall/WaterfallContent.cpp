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

WfColorMap* WaterfallContent::getColorMap()
{
	WfColorMap* colorMap = nullptr;
	readWriteLock->lockForRead();
	if(waterfallLayer)
	{
		colorMap = waterfallLayer->colorMap;
	} 
	readWriteLock->unlock();
	return colorMap;
}

void WaterfallContent::setAppendSide(EAppendSide side)
{
	readWriteLock->lockForWrite();

	appendSide = side;

	readWriteLock->unlock();
}

void WaterfallContent::updatePixmap()
{
	readWritePixmap->lockForWrite();
	const QPixmap newPixmap = QPixmap::fromImage(*waterfallLayer->image);
	if (newPixmap.isNull()) qDebug() << "pixmap is null!!!" << waterfallLayer->image->rect();
	setPixmap(newPixmap);
	readWritePixmap->unlock();
}

void WaterfallContent::update()
{
	parentPlot()->layer(WATERFALL_LAYER_NAME)->replot();
}

void WaterfallContent::setResolution(int width, int height)
{
	qDebug() << "set resolution";
	readWriteLock->lockForRead();
	if ((waterfallLayer->image->width() == width
		&& waterfallLayer->image->height() == height) || (width == 0 || height == 0))
	{
		qDebug() << parentQtPlot->objectName() << "| Set Resolution(" << width << "," << height << ") error";
		readWriteLock->unlock();
		return;
	}
	readWriteLock->unlock();

	readWriteLock->lockForWrite();

	delete waterfallLayer->image;
	waterfallLayer->image = new QImage(width, height, waterfallLayer->format);
	if(waterfallLayer->image == nullptr || 
		waterfallLayer->image->height() <= 0 || waterfallLayer->image->width() <= 0)
	{
		qDebug() << "Can't create Image(" << width << "," << height << ")";
	}
	waterfallLayer->image->fill(waterfallLayer->fillColor);
	update();

	readWriteLock->unlock();
}

QRect WaterfallContent::getResolution() const
{
	QRect resolution;

	readWriteLock->lockForRead();
	if (waterfallLayer)
	{
		resolution = waterfallLayer->image->rect();
	}
	readWriteLock->unlock();

	return resolution;
}

void WaterfallContent::setWidth(int width)
{
	if (waterfallLayer->image->width() == width || width <= 0) 
	{
		qDebug() << parentQtPlot->objectName() << "| Set Width(" << width << ") error";
		return;
	}

	readWriteLock->lockForRead();
	const int height = waterfallLayer->image->height();
	readWriteLock->unlock();

	setResolution(width, height);
}

void WaterfallContent::setHeight(int height)
{
	if (waterfallLayer->image->height() == height || height <= 0) 
	{
		qDebug() << parentQtPlot->objectName() << "| Set Height(" << height << ") error";
		return;
	}

	readWriteLock->lockForRead();
	const int width = waterfallLayer->image->width();
	readWriteLock->unlock();

	setResolution(width, height);
}

void WaterfallContent::setFillColor(const QColor& fillColor)
{
	readWriteLock->lockForWrite();
	
	waterfallLayer->fillColor = fillColor;
	waterfallLayer->image->fill(fillColor);
	updatePixmap();

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
		updatePixmap();

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
	updatePixmap();

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
		updatePixmap();
	}

	readWriteLock->unlock();
}

void WaterfallContent::setData(double* data, int width, int height)
{
	readWriteLock->lockForRead();

	switch (appendSide)
	{
	case EAS_Top:
	{
		setFullDataT(data, width, height, appendHeight);
		break;
	}

	case EAS_Bottom:
	{
		setFullDataB(data, width, height, appendHeight);
		break;
	}

	case EAS_Left:
	{
		setFullDataL(data, width, height, appendHeight);
		break;
	}

	case EAS_Right:
	{
		setFullDataR(data, width, height, appendHeight);
		break;
	}
	}

	updatePixmap();

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

	updatePixmap();

	waterfallLayer->range = QtInterval(minval, maxval);

	topLeft->setCoords(minx, maxy);
	bottomRight->setCoords(maxx, miny);

	readWriteLock->unlock();

	return true;
}

void WaterfallContent::appendT(double* data, int w, int h)
{
	if (waterfallLayer->image->width() > w)
	{
		qDebug() << "Error append T" << waterfallLayer->image->width() << w;
		return;
	}

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
	if (waterfallLayer->image->width() > w)
	{
		qDebug() << "Error append B";
		return;
	}

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
	if (waterfallLayer->image->height() > w) 
	{
		qDebug() << "Error append L";
		return;
	}

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
	if (waterfallLayer->image->height() > w)
	{
		qDebug() << "Error append R";
		return;
	}

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

//todo: now don't using appendHeight 
void WaterfallContent::setFullDataT(double* data, int w, int h, int /*appendHeight*/)
{
	if (waterfallLayer->image->width() < w ||
		waterfallLayer->image->height() < h) 
	{
		qDebug() << "Error set full data T";
		return;
	}

	for(int y = 0; y < h; y++)
	{
		QRgb* line = reinterpret_cast<QRgb*>(waterfallLayer->image->scanLine(y));
		const int offset = (h - 1 - y) * w;

		for (int x = 0; x < w; x++)
		{
			*line++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[x + offset]);
		}
	}
}

void WaterfallContent::setFullDataB(double* data, int w, int h, int appendHeight)
{
	if (waterfallLayer->image->width() != w ||
		waterfallLayer->image->height() < h)
	{
		qDebug() << "Error set full data B";
		return;
	}

	for (int y = h - 1; y >= 0; y--)
	{
		QRgb* line = reinterpret_cast<QRgb*>(waterfallLayer->image->scanLine(y));
		const int offset = y * w;

		for (int x = 0; x < w; x++)
		{
			*line++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[x + offset]);
		}
	}
}

//todo: now don't using appendHeight
void WaterfallContent::setFullDataR(double* data, int w, int h, int /*appendHeight*/)
{
	if (waterfallLayer->image->width() != w ||
		waterfallLayer->image->height() < h) 
	{
		qDebug() << "Error set full data R";
		return;
	}

	for (int y = 0; y < h; y++)
	{
		QRgb* line = reinterpret_cast<QRgb*>(waterfallLayer->image->scanLine(y));
		for (int x = 0; x < w; x++)
		{
			*line++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[x*h + y]);
		}
	}
}

void WaterfallContent::setFullDataL(double* data, int w, int h, int appendHeight)
{
	if (waterfallLayer->image->width() != w ||
		waterfallLayer->image->height() < h)
	{
		qDebug() << "Error set full data L";
		return;
	}

	for (int y = 0; y < h; y++)
	{
		QRgb* line = reinterpret_cast<QRgb*>(waterfallLayer->image->scanLine(y));
		for (int x = w - 1; x >= 0; x--)
		{
			*line++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[x * h + y]);
		}
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

			int width = mPixmap.width() * xMultiply;
			if (width < 1) width = 1;

			int height = mPixmap.height() * yMultiply;
			if (height < 1) height = 1;

			const int xOffset = (xRange.lower - xLimitRange.lower) / xDelta * mPixmap.width();
			const int yOffset = (yLimitRange.upper - yRange.upper) / yDelta * mPixmap.height();
			
			const QRect copyRect(xOffset, yOffset, width, height);

			readWritePixmap->lockForRead();
			mScaledPixmap = mPixmap.copy(copyRect);
			readWritePixmap->unlock();

			mScaledPixmap = mScaledPixmap.scaled(clipRect().size() * devicePixelRatio, mAspectRatioMode, mTransformationMode);

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
