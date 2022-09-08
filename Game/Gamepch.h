
#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

// Windows 헤더 파일
#include <windows.h>
#include <memory>
#include <string>
#include "resource.h"

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b)										\
	{													\
			OutputDebugStringA("Assert: " #b "\n");		\
			MessageBox(NULL, b, L"Error", MB_OK);		\
	}		
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#endif //PCH_H
