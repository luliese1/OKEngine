#pragma once

#ifdef OKENGINE_EXPORTS
#define  OKENGINE_API __declspec(dllexport)
#else
#define  OKENGINE_API __declspec(dllimport)
#endif