#pragma once

#include <QCustomPlot/QCustomPlot.h>


class QTPLOT_EXPORT SettingingPlot : public QCustomPlot
{
	Q_OBJECT

public:
	explicit SettingingPlot(QWidget* parent = nullptr);
	~SettingingPlot();

protected slots:
	virtual void configureSlot(const QString& objectName);

};
