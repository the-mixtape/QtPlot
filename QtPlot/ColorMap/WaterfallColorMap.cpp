#include "WaterfallColorMap.h"

WaterfallColorMap::WaterfallColorMap()
	:LinearColorMap(Qt::black, Qt::red)
{
	setMode(Mode::FixedColors);

	const double size = 255;
	double value, tcolor;
	QColor color;
	for (int i = 1; i < 250; i++)
	{
		{
			value = size / 50.;
			tcolor = value * i - 30;
			if (tcolor < 0) tcolor = 0;
			if (tcolor > 255) tcolor = 511 - tcolor;
			if (i > 100)
				color.setBlue(50);
			else
				color.setBlue(tcolor);
		}

		{
			value = size / 120.0;
			tcolor = value * i - 50;
			if (tcolor < 0) tcolor = 0;
			if (tcolor > 255) tcolor = 511 - tcolor;
			if (i > 250)
				color.setGreen(25);
			else
				color.setGreen(tcolor);
		}

		{
			value = size / 185.0;
			tcolor = value * i - 80;
			if (tcolor < 0) tcolor = 0;
			if (tcolor > 255) tcolor = 511 - tcolor;
			color.setRed(tcolor);
		}

		addColorStop(static_cast<double>(i) / 255.0, color);
	}
}
