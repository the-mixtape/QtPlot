#include "WaterfallThread.h"
#include "WaterfallContent.h"


WaterfallThread::WaterfallThread(QObject* object)
	:QThread(object),
	data(nullptr),
	size(0),
	frameDeltaTime(0),
	bIsAuto(true)
{
	appendMutex.lock();
	frameTimer = new QElapsedTimer;
}

WaterfallThread::~WaterfallThread()
{
	stopAndClear();

	delete data;
	delete frameTimer;
}

void WaterfallThread::run()
{
	bIsQuit = false;
	qint64 delta;
	qint64 sleepTime;

	while(!bIsQuit)
	{
		frameTimer->start();
		appendMutex.lock();

		//lock fps logic
		{
			locker.lockForRead();

			delta = frameTimer->elapsed();
			sleepTime = frameDeltaTime - delta;

			if (sleepTime > 0)
			{
				msleep(sleepTime);
			}

			locker.unlock();
		}

		if (bIsQuit) return;

		if (content)
		{
			locker.lockForRead();

			content->append(data, size, bIsAuto);

			if(bIsAuto)
			{
				emit update();
			}

			locker.unlock();
		}

		copyMutex.unlock();
	}
}

void WaterfallThread::quit()
{
	bIsQuit = true;
	QThread::quit();
}

void WaterfallThread::stopAndClear()
{
	quit();
	wait();
}

void WaterfallThread::setAutoUpdate(bool bAuto)
{
	locker.lockForWrite();
	bIsAuto = bAuto;
	locker.unlock();
}

void WaterfallThread::setFPSLimit(quint32 fps /*= 0*/)
{
	locker.lockForWrite();

	if(fps <= 0)
	{
		frameDeltaTime = 0;
	}
	else
	{
		frameDeltaTime = static_cast<qint64>(1000 / static_cast<double>(fps));
	}

	locker.unlock();
}

void WaterfallThread::addData(double* inData, int inSize)
{
	copyMutex.lock();

	if (size != inSize)
	{ 
		delete data;
		data = new double[inSize];
		size = inSize;
	}

	if(data)
	{
		memcpy(data, inData, size * sizeof(double));
	}

	emit copyingCompleted();
	appendMutex.unlock();
}

void WaterfallThread::setWaterfallContent(WaterfallContent* inContent)
{
	if (!inContent) return;

	content = inContent;
	connect(this, &WaterfallThread::update, content, &WaterfallContent::update);
}
