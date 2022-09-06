#pragma once

#include <QThread>
#include <QMutex>


class WaterfallContent;


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
	void setWaterfallContent(WaterfallContent* content);

signals:
	void update();

private:
	QMutex appendMutex;
	QMutex copyMutex;

	WaterfallContent* content;

	double* data;
	int size;

	bool bIsQuit;

};

