#pragma once

#include "QtPlotGlobal.h"



enum EAxis
{
	EA_xAxis,
	EA_yAxis
};

enum ELineState
{
	idle,
	hovered,
	dragging
};

enum EAppendSide
{
	EAS_Top,
	EAS_Bottom,
	EAS_Left,
	EAS_Right
};