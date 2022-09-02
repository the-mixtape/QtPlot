#pragma once

#include <qrgb.h>
#include <QVector>

#include "Interval.h"
#include "QtPlotGlobal.h"

class QTPLOT_EXPORT ColorMap
{
public:
	enum Format
	{
		RGB,
		Indexed
	};

	explicit ColorMap(Format = RGB);
	virtual ~ColorMap();

	void setFormat(Format);
	Format format() const;

	virtual QRgb rgb(const QtInterval& interval, double value) const = 0;
	virtual uint colorIndex(int numColors, const QtInterval& interval, double value) const;

	QColor color(const QtInterval&, double value) const;
	virtual QVector<QRgb> colorTable(int numColors) const;
	virtual QVector<QRgb> colorTable256() const;

private:
	Q_DISABLE_COPY(ColorMap)

	Format _format;

};

class QTPLOT_EXPORT LinearColorMap : public ColorMap
{
public:
    enum Mode
    {
        //! Return the color from the next lower color stop
        FixedColors,

        //! Interpolating the colors of the adjacent stops.
        ScaledColors
    };

    explicit LinearColorMap(ColorMap::Format = ColorMap::RGB);

    LinearColorMap(const QColor& from, const QColor& to,
        ColorMap::Format = ColorMap::RGB);

    ~LinearColorMap() override;

    void setMode(Mode);
    Mode mode() const;

    void setColorInterval(const QColor& color1, const QColor& color2);
    void addColorStop(double value, const QColor&);
    QVector<double> colorStops() const;

    QColor color1() const;
    QColor color2() const;

    virtual QRgb rgb(const QtInterval&,
        double value) const override;

    virtual uint colorIndex(int numColors,
        const QtInterval&, double value) const override;

    class ColorStops;

private:
    class PrivateData;
    PrivateData* m_data;
};