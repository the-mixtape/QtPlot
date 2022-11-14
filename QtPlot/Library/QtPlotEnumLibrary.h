#pragma once


enum EAxis
{
	EA_xAxis,
	EA_yAxis
};

enum ELineState
{
	ELS_Idle,
	ELS_Hovered,
	ELS_Dragging
};

enum EAppendSide
{
	EAS_Top,
	EAS_Bottom,
	EAS_Left,
	EAS_Right
};

enum ESyncRule
{
	ESR_Percentage,
	ESR_Range
};

enum EShowInfoRule
{
	ESIR_None,
	ESIR_Attach,
};

enum EClickPos
{
	ECA_ClickX,
	ECA_ClickY,
	ECA_ClickPlot,
};