#include "SettingingPlot.h"


SettingingPlot::SettingingPlot(QWidget* parent) : QCustomPlot(parent)
{
	connect(this, &QObject::objectNameChanged, this, &SettingingPlot::configureSlot);
}

SettingingPlot::~SettingingPlot()
{
}

void SettingingPlot::configureSlot(const QString& objectName)
{
}
