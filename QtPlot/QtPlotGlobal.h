#pragma once

#include <QtCore/qglobal.h>

#define WATERFALL_LAYER_NAME	"waterfall"
#define MARKERS_LAYER_NAME		"markers"

#ifndef BUILD_STATIC
# if defined(QTPLOT_LIB)
#  define QTPLOT_EXPORT Q_DECL_EXPORT
# else
#  define QTPLOT_EXPORT Q_DECL_IMPORT
# endif
#else
# define QTPLOT_EXPORT
#endif
