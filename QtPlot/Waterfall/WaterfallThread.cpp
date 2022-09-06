#include "WaterfallThread.h"

#include "WaterfallContent.h"


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
	double* testData = new double[512];

	while(!bIsQuit)
	{
		// appendMutex.lock();

		if (bIsQuit) return;
		{
			//append logic

			//test logic
			{
				if (content) 
				{
					msleep(99);
					for(int i = 0; i < 512; i++)
					{
						testData[i] = i;
						//rand() % 100;
					}

					content->appendL(testData, 512, 3);
					emit update();
				}
			}

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

void WaterfallThread::setWaterfallContent(WaterfallContent* inContent)
{
	if (!inContent) return;

	content = inContent;
	connect(this, &WaterfallThread::update, content, &WaterfallContent::update);
}
