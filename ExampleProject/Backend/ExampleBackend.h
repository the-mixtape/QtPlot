#pragma once
#include <QMutex>
#include <QThread>

class ExampleBackend : public QThread
{
	Q_OBJECT

public:
	ExampleBackend(QObject* parent = nullptr);
	~ExampleBackend();

	void stopAndClear();
	void quit();

signals:
	void generatedNewData(double* data, int size);

public slots:
	void copyingCompleted();

protected:
	void run() override;

private:
	QMutex generateNewData;
	bool bIsQuit;

	int size = 512;
	double* data;

};

