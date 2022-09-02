#include "WaterfallThread.h"



WaterfallThread::WaterfallThread(QObject* object)
	:QThread(object), data(nullptr), size(0)
{
	appendMutex.lock();
}

WaterfallThread::~WaterfallThread()
{
	stopAndClear();

	delete data;
}

void WaterfallThread::run()
{
	bIsQuit = false;
	while(!bIsQuit)
	{
		appendMutex.lock();

		if (bIsQuit) return;
		{
			//append logic
		}
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
		memcpy(inData, data, size);
	}

	copyMutex.unlock();
	appendMutex.unlock();
}
