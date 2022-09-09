#pragma once

#include "QtPlotGlobal.h"
#include <qcolor.h>


class QtInterval;

#if QT_VERSION < 0x060000
template< typename T > class QVector;
#endif

class QTPLOT_EXPORT WfColorMap
{
public:
	enum Format
	{
		RGB,
		Indexed
	};

	explicit WfColorMap(Format = RGB);
	virtual ~WfColorMap();

	void setFormat(Format);
	Format format() const;

	virtual QRgb rgb(const QtInterval& interval, double value) const = 0;
	virtual uint colorIndex(int numColors, const QtInterval& interval, double value) const;

	QColor color(const QtInterval&, double value) const;
	virtual QVector<QRgb> colorTable(int numColors) const;
	virtual QVector<QRgb> colorTable256() const;

private:
	Q_DISABLE_COPY(WfColorMap)
	Format _format;

};

class QTPLOT_EXPORT LinearColorMap : public WfColorMap
{
public:
    enum Mode
    {
        //! Return the color from the next lower color stop
        FixedColors,

        //! Interpolating the colors of the adjacent stops.
        ScaledColors
    };

    explicit LinearColorMap(WfColorMap::Format = WfColorMap::RGB);

    LinearColorMap(const QColor& from, const QColor& to,
        WfColorMap::Format = WfColorMap::RGB);

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