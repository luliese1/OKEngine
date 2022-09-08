#include "Gamepch.h"
#include "WinMain.h"
#include "WinProc.h"

//#include <vld.h>

/// <summary>
/// 
/// 2022-06-08
/// ���� ���� ������.
/// 
/// </summary>

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h> // �޸� �Ҵ� �� �Ҵ� ������ �����ϴ� �Լ����� ���ǵǾ� ����.
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
		Assert(L"�۾����丮 ���� ����");
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
