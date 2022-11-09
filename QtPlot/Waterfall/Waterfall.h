#pragma once

#include "WaterfallBase.h"


class QTPLOT_EXPORT WaterfallPlot : public WaterfallBase
{
	Q_OBJECT

public:
	explicit WaterfallPlot(QWidget* parent);
	~WaterfallPlot() override;
};

