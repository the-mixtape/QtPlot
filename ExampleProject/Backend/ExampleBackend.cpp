#include "ExampleBackend.h"

#include <QThread>
#include <QDebug>


ExampleBackend::ExampleBackend(QObject* parent)
	:QThread(parent)
{
	connect(this, &QThread::finished, [=]() { deleteLater(); });
}

ExampleBackend::~ExampleBackend()
{
	// delete data;
}

void ExampleBackend::stopAndClear()
{
	quit();
	wait();
}

void ExampleBackend::quit()
{
	bIsQuit = true;
	generateNewData.unlock();

	QThread::quit();
}

void ExampleBackend::copyingCompleted()
{
	generateNewData.unlock();
}

void ExampleBackend::run()
{
	data = new double[size];
	double d = 1;
	bIsQuit = false;
	while(!bIsQuit)
	{
		generateNewData.lock();

		if (bIsQuit) break;

		d = (rand() % 4 + 2);
		for(int i = 0; i < size; i++)
		{
			data[i] = sin(i / (size/d)) * 100.0;
		}

		emit generatedNewData(data, size);
	}

	generateNewData.unlock();
}
