
#ifndef PCH_H
#define PCH_H

// ���⿡ �̸� �������Ϸ��� ��� �߰�
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

// Windows ��� ����
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
