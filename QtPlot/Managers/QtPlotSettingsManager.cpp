#include "Managers/QtPlotSettingsManager.h"
#include <QSettings>

#define CONFIG_FILE     "QtPlotConfig.ini"
#define GROUP           "SETTINGS"

#define GRID "Grid"


QtPlotSettingsManager::QtPlotSettingsManager()
	:QtPlotSingleton<QtPlotSettingsManager>(*this)
{
    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat);
    settings->beginGroup(GROUP);
}

QtPlotSettingsManager::~QtPlotSettingsManager()
{
    delete settings;
}

void QtPlotSettingsManager::setGridVisible(const QString& objectName, bool enable)
{
    locker.lockForWrite();
    settings->setValue(objectName + GRID, enable);
    locker.unlock();
}

bool QtPlotSettingsManager::getGridVisible(const QString& objectName)
{
    locker.lockForRead();
    const bool enable = settings->value(objectName + GRID, false).toBool();
    locker.unlock();

    return enable;
}
