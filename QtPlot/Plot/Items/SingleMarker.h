#pragma once

#include "BaseMarker.h"


class QTPLOT_EXPORT SingleMarker : public BaseMarker
{
	Q_OBJECT

public:
	SingleMarker(QCustomPlot* parentPlot);

	void setMarkerPos(int pos);

	void showMarker();
	void hideMarker();

private:
	void xMoveAxis(QMouseEvent* event) override;
	void yMoveAxis(QMouseEvent* event) override;

signals:
	void updateMarkerPosSignal(int pos);
};

