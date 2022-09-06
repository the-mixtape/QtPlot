#include "WaterfallContent.h"

#include "WfColorMap.h"
#include "WaterfallLayer.h"


WaterfallContent::WaterfallContent(QCustomPlot* parent)
	: QCPItemPixmap(parent), waterfallLayer(nullptr)
{
	readWriteLock = new QReadWriteLock(QReadWriteLock::Recursive);
	setScaled(true, Qt::IgnoreAspectRatio);
}

WaterfallContent::~WaterfallContent()
{
	disconnect();

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

void WaterfallContent::update()
{
	parentPlot()->layer(WATERFALL_LAYER_NAME)->replot();
}

bool WaterfallContent::addLayer(qint32 inWidth, qint32 inHeight, qreal minx, qreal miny, qreal maxx, qreal maxy,
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
	readWriteLock->lockForRead();

	if (!readWriteLock || !waterfallLayer) return;

	if (waterfallLayer->image.width() >= w && waterfallLayer->image.height() >= h)
	{
		int y = 0;
		uchar* data_ = waterfallLayer->image.bits();
		memmove(data_ + waterfallLayer->image.bytesPerLine() * h, data_, waterfallLayer->image.sizeInBytes() - waterfallLayer->image.bytesPerLine() * h);
		while (y < h)
		{
			QRgb* line = reinterpret_cast<QRgb*>(waterfallLayer->image.scanLine(y));
			for (int x = 0; x < w; x++)
			{
				*line++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[x]);
			}
			y++;
		}
		setPixmap(QPixmap::fromImage(waterfallLayer->image));
	}

	readWriteLock->unlock();
}

void WaterfallContent::appendB(double* data, int w, int h)
{
	int	y = h;
	readWriteLock->lockForRead();

	if (waterfallLayer->image.width() >= w && waterfallLayer->image.height() >= h) {
		uchar* data_ = waterfallLayer->image.bits();
		memmove(data_, data_ + waterfallLayer->image.bytesPerLine() * h, waterfallLayer->image.sizeInBytes() - waterfallLayer->image.bytesPerLine() * h);
		while (y >= 1) {
			QRgb* line = reinterpret_cast<QRgb*>(waterfallLayer->image.scanLine(waterfallLayer->image.height() - y));
			for (int x = 0; x < w; x++)
				*line++ = waterfallLayer->colorMap->rgb(waterfallLayer->range, data[x]);
			y--;
		}
		setPixmap(QPixmap::fromImage(waterfallLayer->image));
	}
	
	readWriteLock->unlock();
}

void WaterfallContent::appendL(double* data, int w, int h)
{
	readWriteLock->lockForRead();

	qint32 bpp = waterfallLayer->image.depth() / 8;
	if (waterfallLayer->image.width() >= h && waterfallLayer->image.height() >= w)
	{
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
		setPixmap(QPixmap::fromImage(waterfallLayer->image));
	}

	readWriteLock->unlock();
}

void WaterfallContent::appendR(double* data, int w, int h)
{
	readWriteLock->lockForRead();

	qint32 bpp = waterfallLayer->image.depth() / 8;
	if (waterfallLayer->image.width() >= h && waterfallLayer->image.height() >= w)
	{
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
		setPixmap(QPixmap::fromImage(waterfallLayer->image));
	}
	
	readWriteLock->unlock();
}
