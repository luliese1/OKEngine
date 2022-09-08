#include "Gamepch.h"
#include "WinMain.h"
#include "WinProc.h"

//#include <vld.h>

/// <summary>
/// 
/// 2022-06-08
/// 엔진 제작 스따뚜.
/// 
/// </summary>

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h> // 메모리 할당 및 할당 해제를 추적하는 함수들이 정의되어 있음.
#endif


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
//#ifdef _DEBUG
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif // DEBUG
//
	//_CrtSetBreakAlloc(444);

	if (SetCurrentDir() == 0)
	{
		Assert(L"작업디렉토리 설정 실패");
		return 0;
	}

	WinProc* pWinProc = new WinProc();

	HRESULT result = pWinProc->Initialize(hInstance);
	if (result == S_OK)
	{
	pWinProc->Loop();

	pWinProc->Finalize();
	}
	delete pWinProc;

	//_CrtDumpMemoryLeaks();

	return 0;
}
