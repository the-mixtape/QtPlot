#pragma once

#include <QThread>
#include <QMutex>


class WaterfallThread : public QThread
{
	Q_OBJECT

public:
	explicit WaterfallThread(QObject* object = nullptr);
	~WaterfallThread() override;

protected:
	void run() override;

public:
	void quit();
	void stopAndClear();

	void addData(double* data, int size);
	void setWaterfall();


private:
	QMutex appendMutex;
	QMutex copyMutex;

	double* data;
	int size;

	bool bIsQuit;

};

