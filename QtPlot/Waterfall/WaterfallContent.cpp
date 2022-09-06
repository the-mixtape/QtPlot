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
	parentPlot()->layer("waterfall")->replot();
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
	int	y = 0;
	readWriteLock->lockForRead();

	if (!readWriteLock || !waterfallLayer) return;

	if (waterfallLayer->image.width() >= w && waterfallLayer->image.height() >= h)
	{
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
