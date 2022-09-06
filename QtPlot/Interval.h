/******************************************************************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_INTERVAL_H
#define QWT_INTERVAL_H

#include "QtPlotGlobal.h"
#include <qmetatype.h>

/*!
   \brief A class representing an interval

   The interval is represented by 2 doubles, the lower and the upper limit.
 */

class QTPLOT_EXPORT QtInterval
{
  public:
    /*!
       Flag indicating if a border is included or excluded
       \sa setBorderFlags(), borderFlags()
     */
    enum BorderFlag
    {
        //! Min/Max values are inside the interval
        IncludeBorders = 0x00,

        //! Min value is not included in the interval
        ExcludeMinimum = 0x01,

        //! Max value is not included in the interval
        ExcludeMaximum = 0x02,

        //! Min/Max values are not included in the interval
        ExcludeBorders = ExcludeMinimum | ExcludeMaximum
    };

    //! Border flags
    Q_DECLARE_FLAGS( BorderFlags, BorderFlag )

    QtInterval();
    QtInterval( double minValue, double maxValue,
        BorderFlags = IncludeBorders );

    void setInterval( double minValue, double maxValue,
        BorderFlags = IncludeBorders );

    QtInterval normalized() const;
    QtInterval inverted() const;
    QtInterval limited( double lowerBound, double upperBound ) const;

    bool operator==( const QtInterval& ) const;
    bool operator!=( const QtInterval& ) const;

    void setBorderFlags( BorderFlags );
    BorderFlags borderFlags() const;

    double minValue() const;
    double maxValue() const;

    double width() const;
    long double widthL() const;

    void setMinValue( double );
    void setMaxValue( double );

    bool contains( double value ) const;
    bool contains( const QtInterval& ) const;

    bool intersects( const QtInterval& ) const;
    QtInterval intersect( const QtInterval& ) const;
    QtInterval unite( const QtInterval& ) const;

    QtInterval operator|( const QtInterval& ) const;
    QtInterval operator&( const QtInterval& ) const;

    QtInterval& operator|=( const QtInterval& );
    QtInterval& operator&=( const QtInterval& );

    QtInterval extend( double value ) const;
    QtInterval operator|( double ) const;
    QtInterval& operator|=( double );

    bool isValid() const;
    bool isNull() const;
    void invalidate();

    QtInterval symmetrize( double value ) const;

  private:
    double m_minValue;
    double m_maxValue;
    BorderFlags m_borderFlags;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QtInterval::BorderFlags)
Q_DECLARE_METATYPE(QtInterval)
Q_DECLARE_TYPEINFO(QtInterval, Q_MOVABLE_TYPE);

/*!
   \brief Default Constructor

   Creates an invalid interval [0.0, -1.0]
   \sa setInterval(), isValid()
 */
inline QtInterval::QtInterval()
    : m_minValue( 0.0 )
    , m_maxValue( -1.0 )
    , m_borderFlags( IncludeBorders )
{
}

/*!
   Constructor

   Build an interval with from min/max values

   \param minValue Minimum value
   \param maxValue Maximum value
   \param borderFlags Include/Exclude borders
 */
inline QtInterval::QtInterval(
        double minValue, double maxValue, BorderFlags borderFlags )
    : m_minValue( minValue )
    , m_maxValue( maxValue )
    , m_borderFlags( borderFlags )
{
}

/*!
   Assign the limits of the interval

   \param minValue Minimum value
   \param maxValue Maximum value
   \param borderFlags Include/Exclude borders
 */
inline void QtInterval::setInterval(
    double minValue, double maxValue, BorderFlags borderFlags )
{
    m_minValue = minValue;
    m_maxValue = maxValue;
    m_borderFlags = borderFlags;
}

/*!
   Change the border flags

   \param borderFlags Or'd BorderMode flags
   \sa borderFlags()
 */
inline void QtInterval::setBorderFlags( BorderFlags borderFlags )
{
    m_borderFlags = borderFlags;
}

/*!
   \return Border flags
   \sa setBorderFlags()
 */
inline QtInterval::BorderFlags QtInterval::borderFlags() const
{
    return m_borderFlags;
}

/*!
   Assign the lower limit of the interval

   \param minValue Minimum value
 */
inline void QtInterval::setMinValue( double minValue )
{
    m_minValue = minValue;
}

/*!
   Assign the upper limit of the interval

   \param maxValue Maximum value
 */
inline void QtInterval::setMaxValue( double maxValue )
{
    m_maxValue = maxValue;
}

//! \return Lower limit of the interval
inline double QtInterval::minValue() const
{
    return m_minValue;
}

//! \return Upper limit of the interval
inline double QtInterval::maxValue() const
{
    return m_maxValue;
}

/*!
   A interval is valid when minValue() <= maxValue().
   In case of QtInterval::ExcludeBorders it is true
   when minValue() < maxValue()

   \return True, when the interval is valid
 */
inline bool QtInterval::isValid() const
{
    if ( ( m_borderFlags & ExcludeBorders ) == 0 )
        return m_minValue <= m_maxValue;
    else
        return m_minValue < m_maxValue;
}

/*!
   \brief Return the width of an interval

   The width of invalid intervals is 0.0, otherwise the result is
   maxValue() - minValue().

   \return Interval width
   \sa isValid()
 */
inline double QtInterval::width() const
{
    return isValid() ? ( m_maxValue - m_minValue ) : 0.0;
}

/*!
   \brief Return the width of an interval as long double

   The width of invalid intervals is 0.0, otherwise the result is
   maxValue() - minValue().

   \return Interval width
   \sa isValid()
 */
inline long double QtInterval::widthL() const
{
    if ( !isValid() )
        return 0.0;

    return static_cast< long double >( m_maxValue )
           - static_cast< long double >( m_minValue );
}

/*!
   \brief Intersection of two intervals

   \param other Interval to intersect with
   \return Intersection of this and other

   \sa intersect()
 */
inline QtInterval QtInterval::operator&(
    const QtInterval& other ) const
{
    return intersect( other );
}

/*!
   Union of two intervals

   \param other Interval to unite with
   \return Union of this and other

   \sa unite()
 */
inline QtInterval QtInterval::operator|(
    const QtInterval& other ) const
{
    return unite( other );
}

/*!
   \brief Compare two intervals

   \param other Interval to compare with
   \return True, when this and other are equal
 */
inline bool QtInterval::operator==( const QtInterval& other ) const
{
    return ( m_minValue == other.m_minValue ) &&
           ( m_maxValue == other.m_maxValue ) &&
           ( m_borderFlags == other.m_borderFlags );
}
/*!
   \brief Compare two intervals

   \param other Interval to compare with
   \return True, when this and other are not equal
 */
inline bool QtInterval::operator!=( const QtInterval& other ) const
{
    return ( !( *this == other ) );
}

/*!
   Extend an interval

   \param value Value
   \return Extended interval
   \sa extend()
 */
inline QtInterval QtInterval::operator|( double value ) const
{
    return extend( value );
}

//! \return true, if isValid() && (minValue() >= maxValue())
inline bool QtInterval::isNull() const
{
    return isValid() && m_minValue >= m_maxValue;
}

/*!
   Invalidate the interval

   The limits are set to interval [0.0, -1.0]
   \sa isValid()
 */
inline void QtInterval::invalidate()
{
    m_minValue = 0.0;
    m_maxValue = -1.0;
}

#ifndef QT_NO_DEBUG_STREAM
QTPLOT_EXPORT QDebug operator<<( QDebug, const QtInterval& );
#endif

#endif
