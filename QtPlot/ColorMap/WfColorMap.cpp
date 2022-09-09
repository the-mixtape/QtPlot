#include "WfColorMap.h"

#include "Interval.h"
#include <qvector.h>


class LinearColorMap::ColorStops
{
public:
    ColorStops() :
        m_doAlpha(false)
    {
        m_stops.reserve(256);
    }

    void insert(double pos, const QColor& color);
    QRgb rgb(LinearColorMap::Mode, double pos) const;

    QVector< double > stops() const;

private:

    class ColorStop
    {
    public:
        ColorStop() :
            pos(0.0),
            rgb(0)
        {
        };

        ColorStop(double p, const QColor& c) :
            pos(p),
            rgb(c.rgba())
        {
            r = qRed(rgb);
            g = qGreen(rgb);
            b = qBlue(rgb);
            a = qAlpha(rgb);

            /*
                when mapping a value to rgb we will have to calculate:
                   - const int v = int( ( s1.v0 + ratio * s1.vStep ) + 0.5 );

                Thus adding 0.5 ( for rounding ) can be done in advance
             */
            r0 = r + 0.5;
            g0 = g + 0.5;
            b0 = b + 0.5;
            a0 = a + 0.5;

            rStep = gStep = bStep = aStep = 0.0;
            posStep = 0.0;
        }

        void updateSteps(const ColorStop& nextStop)
        {
            rStep = nextStop.r - r;
            gStep = nextStop.g - g;
            bStep = nextStop.b - b;
            aStep = nextStop.a - a;
            posStep = nextStop.pos - pos;
        }

        double pos;
        QRgb rgb;
        int r, g, b, a;

        // precalculated values
        double rStep, gStep, bStep, aStep;
        double r0, g0, b0, a0;
        double posStep;
    };

    inline int findUpper(double pos) const;
    QVector< ColorStop > m_stops;
    bool m_doAlpha;
};

void LinearColorMap::ColorStops::insert(double pos, const QColor& color)
{
    // Lookups need to be very fast, insertions are not so important.
    // Anyway, a balanced tree is what we need here. TODO ...

    if (pos < 0.0 || pos > 1.0)
        return;

    int index;
    if (m_stops.size() == 0)
    {
        index = 0;
        m_stops.resize(1);
    }
    else
    {
        index = findUpper(pos);
        if (index == m_stops.size() ||
            qAbs(m_stops[index].pos - pos) >= 0.001)
        {
            m_stops.resize(m_stops.size() + 1);
            for (int i = m_stops.size() - 1; i > index; i--)
                m_stops[i] = m_stops[i - 1];
        }
    }

    m_stops[index] = ColorStop(pos, color);
    if (color.alpha() != 255)
        m_doAlpha = true;

    if (index > 0)
        m_stops[index - 1].updateSteps(m_stops[index]);

    if (index < m_stops.size() - 1)
        m_stops[index].updateSteps(m_stops[index + 1]);
}

inline QVector< double > LinearColorMap::ColorStops::stops() const
{
    QVector< double > positions(m_stops.size());
    for (int i = 0; i < m_stops.size(); i++)
        positions[i] = m_stops[i].pos;
    return positions;
}

inline int LinearColorMap::ColorStops::findUpper(double pos) const
{
    int index = 0;
    int n = m_stops.size();

    const ColorStop* stops = m_stops.data();

    while (n > 0)
    {
        const int half = n >> 1;
        const int middle = index + half;

        if (stops[middle].pos <= pos)
        {
            index = middle + 1;
            n -= half + 1;
        }
        else
            n = half;
    }

    return index;
}

inline QRgb LinearColorMap::ColorStops::rgb(
    LinearColorMap::Mode mode, double pos) const
{
    if (pos <= 0.0)
        return m_stops[0].rgb;
    if (pos >= 1.0)
        return m_stops[m_stops.size() - 1].rgb;

    const int index = findUpper(pos);
    if (mode == LinearColorMap::FixedColors)
    {
        return m_stops[index - 1].rgb;
    }
    else
    {
        const ColorStop& s1 = m_stops[index - 1];

        const double ratio = (pos - s1.pos) / (s1.posStep);

        const int r = int(s1.r0 + ratio * s1.rStep);
        const int g = int(s1.g0 + ratio * s1.gStep);
        const int b = int(s1.b0 + ratio * s1.bStep);

        if (m_doAlpha)
        {
            if (s1.aStep)
            {
                const int a = int(s1.a0 + ratio * s1.aStep);
                return qRgba(r, g, b, a);
            }
            else
            {
                return qRgba(r, g, b, s1.a);
            }
        }
        else
        {
            return qRgb(r, g, b);
        }
    }
}



WfColorMap::WfColorMap(Format format) : _format(format)
{
}

WfColorMap::~WfColorMap()
{
}

void WfColorMap::setFormat(Format format)
{
    _format = format;
}

WfColorMap::Format WfColorMap::format() const
{
    return _format;
}

uint WfColorMap::colorIndex(int numColors, const QtInterval& interval, double value) const
{
    const double width = interval.width();
    if (width <= 0.0)
        return 0;

    if (value <= interval.minValue())
        return 0;

    const int maxIndex = numColors - 1;
    if (value >= interval.maxValue())
        return maxIndex;

    const double v = maxIndex * ((value - interval.minValue()) / width);
    return static_cast<unsigned int>(v + 0.5);
}

QVector<QRgb> WfColorMap::colorTable(int numColors) const
{
    QVector<QRgb> table(256);

    const QtInterval interval(0, 256);

    for (int i = 0; i < 256; i++)
        table[i] = rgb(interval, i);

    return table;
}

QVector<QRgb> WfColorMap::colorTable256() const
{
    QVector< QRgb > table(256);

    const QtInterval interval(0, 256);

    for (int i = 0; i < 256; i++)
        table[i] = rgb(interval, i);

    return table;
}

class LinearColorMap::PrivateData
{
public:
    ColorStops colorStops;
    LinearColorMap::Mode mode;
};


LinearColorMap::LinearColorMap(WfColorMap::Format format):
	WfColorMap(format)
{
    m_data = new PrivateData;
    m_data->mode = ScaledColors;

    setColorInterval(Qt::blue, Qt::yellow);
}

LinearColorMap::LinearColorMap(const QColor& from, const QColor& to, WfColorMap::Format format)
    : WfColorMap(format)
{
    m_data = new PrivateData;
    m_data->mode = ScaledColors;
    setColorInterval(from, to);
}

LinearColorMap::~LinearColorMap()
{
    delete m_data;
}

void LinearColorMap::setMode(Mode mode)
{
    m_data->mode = mode;
}

LinearColorMap::Mode LinearColorMap::mode() const
{
    return m_data->mode;
}

void LinearColorMap::setColorInterval(const QColor& color1, const QColor& color2)
{
    m_data->colorStops = ColorStops();
    m_data->colorStops.insert(0.0, color1);
    m_data->colorStops.insert(1.0, color2);
}

void LinearColorMap::addColorStop(double value, const QColor& color)
{
    if (value >= 0.0 && value <= 1.0)
        m_data->colorStops.insert(value, color);
}

QVector<double> LinearColorMap::colorStops() const
{
    return m_data->colorStops.stops();
}

QColor LinearColorMap::color1() const
{
    return QColor::fromRgba(m_data->colorStops.rgb(m_data->mode, 0.0));
}

QColor LinearColorMap::color2() const
{
    return QColor::fromRgba(m_data->colorStops.rgb(m_data->mode, 1.0));
}

QRgb LinearColorMap::rgb(const QtInterval& interval, double value) const
{
    const double width = interval.width();
    if (width <= 0.0)
        return 0u;

    const double ratio = (value - interval.minValue()) / width;
    return m_data->colorStops.rgb(m_data->mode, ratio);
}

uint LinearColorMap::colorIndex(int numColors, const QtInterval& interval, double value) const
{
    const double width = interval.width();
    if (width <= 0.0)
        return 0;

    if (value <= interval.minValue())
        return 0;

    if (value >= interval.maxValue())
        return numColors - 1;

    const double v = (numColors - 1) * (value - interval.minValue()) / width;
    return static_cast<unsigned int>((m_data->mode == FixedColors) ? v : v + 0.5);
}
