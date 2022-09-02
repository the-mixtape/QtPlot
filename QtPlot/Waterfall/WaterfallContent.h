#pragma once

#include <QWidget>


class WaterfallContent : public QWidget
{
	Q_OBJECT

public:
	explicit WaterfallContent(QWidget* parent = nullptr);
	~WaterfallContent() override;
};

