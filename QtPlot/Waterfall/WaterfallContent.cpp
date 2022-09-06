#include "WaterfallContent.h"

#include "WfColorMap.h"
#include "WaterfallLayer.h"


WaterfallContent::WaterfallContent(QCustomPlot* parent)
	: QCPItemPixmap(parent),
	waterfallLayer(nullptr),
	appendSide(EAS_Top),
	appendHeight(1)
{
	readWriteLock = new QReadWriteLock(QReadWriteLock::Recursive);
	setScaled(true, Qt::IgnoreAspectRatio);
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

void WaterfallContent::update()
{
	parentPlot()->layer(WATERFALL_LAYER_NAME)->replot();
}

void WaterfallContent::setResolution(int width, int height)
{
	waterfallLayer->image = QImage(width, height, waterfallLayer->format);
	waterfallLayer->image.fill(waterfallLayer->fillColor);
	update();
}

void WaterfallContent::setFillColor(QColor fillColor)
{
	readWriteLock->lockForWrite();
	
	waterfallLayer->fillColor = fillColor;
	waterfallLayer->image.fill(fillColor);
	setPixmap(QPixmap::fromImage(waterfallLayer->image));

	readWriteLock->unlock();

	update();
}

void WaterfallContent::setInterval(int minval, int maxval)
{
	readWriteLock->lockForWrite();
	
	waterfallLayer->range = QtInterval(minval, maxval);

	readWriteLock->unlock();
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
	
	waterfallLayer->image.fill(waterfallLayer->fillColor);
	setPixmap(QPixmap::fromImage(waterfallLayer->image));

	readWriteLock->unlock();

	update();
}

void WaterfallContent::append(double* data, int w)
{
	readWriteLock->lockForRead();

	if (waterfallLayer->image.width() >= w && waterfallLayer->image.height() >= appendHeight)
	{
		switch (appendSide)
		{
			case EAS_Top:
			{
				appendT(data, w, appendHeight);
				break;
			}

			case EAS_Bottom:
			{
				appendB(data, w, appendHeight);
				break;
			}

			case EAS_Left:
			{
				appendL(data, w, appendHeight);
				break;
			}

			case EAS_Right:
			{
				appendR(data, w, appendHeight);
				break;
			}
		}

		setPixmap(QPixmap::fromImage(waterfallLayer->image));
	}

	readWriteLock->unlock();
}

bool WaterfallContent::createLayer(qint32 inWidth, qint32 inHeight, qreal minx, qreal miny, qreal maxx, qreal maxy,
                                qreal minval, qreal maxval, QImage::Format fm, QColor fil)
{
	delete waterfallLayer;

	readWriteLock->lockForWrite();

	waterfallLayer = new WaterfallLayer();
	waterfallLayer->image = QImage(inWidth, inHeight, fm);
	waterfallLayer->format = fm;
	waterfallLayer->image.fill(fil);
	waterfallLayer->fillColor = fil;
	
	setPixmap(QPixmap::fromImage(waterfallLayer->image));

	waterfallLayer->range = QtInterval(minval, maxval);

	topLeft->setCoords(minx, maxy);
	bottomRight->setCoords(maxx, miny);

	readWriteLock->unlock();

	return true;
}

void WaterfallContent::appendT(double* data, int w, int h)
{
	int y = 0;
	uchar* imageData = waterfallLayer->image.bits();
	memmove(imageData + waterfallLayer->image.bytesPerLine() * h, imageData, waterfallLayer->image.sizeInBytes() - waterfallLayer->image.bytesPerLine() * h);
	while (y < h)
	{
		QRgb* line = reinterpret_cast<QRgb*>(waterfallLayer->image.scanLine(y));
		for (int x = 0; x < w; x++)
		{
			*line++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[x]);
		}
		y++;
	}
}

void WaterfallContent::appendB(double* data, int w, int h)
{
	int	y = h;
	uchar* imageData = waterfallLayer->image.bits();
	memmove(imageData, imageData + waterfallLayer->image.bytesPerLine() * h, waterfallLayer->image.sizeInBytes() - waterfallLayer->image.bytesPerLine() * h);
	while (y >= 1) 
	{
		QRgb* line = reinterpret_cast<QRgb*>(waterfallLayer->image.scanLine(waterfallLayer->image.height() - y));
		for (int x = 0; x < w; x++)
			*line++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[x]);
		y--;
	}
}

void WaterfallContent::appendL(double* data, int w, int h)
{
	const qint32 bpp = waterfallLayer->image.depth() / 8;
	qint32 offset = 0;

	for (int i = 0; i < w; i++)
	{
		uchar* line = waterfallLayer->image.scanLine(i);
		memmove(line + h * bpp, line, waterfallLayer->image.bytesPerLine() - h * bpp);
		QRgb* lina = reinterpret_cast<QRgb*>(line);
		for (int x = offset; x <= offset + h; x++)
			*lina++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[i]);
		offset += h;
	}
}

void WaterfallContent::appendR(double* data, int w, int h)
{
	const qint32 bpp = waterfallLayer->image.depth() / 8;
	qint32 offset = 0;

	for (int i = 0; i < w; i++)
	{
		uchar* line = waterfallLayer->image.scanLine(i);
		memmove(line, line + h * bpp, waterfallLayer->image.bytesPerLine() - h * bpp);
		QRgb* lina = reinterpret_cast<QRgb*>(line + waterfallLayer->image.bytesPerLine() - h * bpp);
		for (int x = offset; x <= offset + h; x++)
			*lina++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[i]);
		offset += h;
	}
}
