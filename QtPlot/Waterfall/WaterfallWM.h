#pragma once

#include "WaterfallBase.h"


class QTPLOT_EXPORT WaterfallWithMemory : public WaterfallBase
{
	Q_OBJECT

public:
	explicit WaterfallWithMemory(QWidget* parent);
	~WaterfallWithMemory() override;

};

