#pragma once
#include <cfloat>
#include <cmath>

inline bool equals(double a, double b)
{
	return (fabs(a - b) < DBL_EPSILON);
}
