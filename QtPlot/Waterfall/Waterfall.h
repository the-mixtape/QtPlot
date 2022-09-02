#pragma once

#include "Plot/QtPlot.h"

//forward declaration
class WaterfallThread;


class QTPLOT_EXPORT WaterfallPlot : public QtPlot
{
public:
	explicit WaterfallPlot(QWidget* parent);
	~WaterfallPlot() override;

private:
	WaterfallThread* loadThread;

};

