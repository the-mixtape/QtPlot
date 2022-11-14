#pragma once

#include "Singleton/QtPlotSingleton.h"

#include <QSettings>
#include <QReadWriteLock>

class QtPlotSettingsManager : public QtPlotSingleton<QtPlotSettingsManager>
{
public:
	QtPlotSettingsManager();
	~QtPlotSettingsManager();

public:
	/*Setters*/
	void setGridVisible(const QString& objectName, bool enable);

	/*Getters*/
	bool getGridVisible(const QString& objectName);

private:
	QReadWriteLock locker;
	QSettings* settings;

};
