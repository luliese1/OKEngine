#pragma once

#ifdef GRAPHICSENGINE_EXPORTS
#define  GRAPHICENGINELIB_API __declspec(dllexport)
#else
#define  GRAPHICENGINELIB_API __declspec(dllimport)
#endif