#include "Gamepch.h"
#include "WinProc.h"

WinProc* WinProc::m_Instance = nullptr;

WinProc::WinProc() :
	m_Msg(), m_hWnd(), m_ClientWidth(0), m_ClientHeight(0), m_tme(), m_hAccelTable(0)
{
	if (m_Instance != nullptr)
	{
		delete this;
	}

	m_Instance = this;
}

WinProc::~WinProc()
{
	if (m_Instance == this)
	{
		m_Instance = nullptr;
	}
}

void WinProc::Loop()
{

	while (true)
	{
		if (PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_Msg.message == WM_QUIT)
				break;

			if (!TranslateAccelerator(m_Msg.hwnd, m_hAccelTable, &m_Msg))
			{
				TranslateMessage(&m_Msg);
				DispatchMessage(&m_Msg);
			}
		}
		GameLoop();
	}
}

void WinProc::GameLoop()
{
	m_GameEngine->Loop();
}

HRESULT WinProc::Initialize(HINSTANCE hInstance)
{
	m_ClientWidth = 1980;
	m_ClientHeight = 960;

	const wchar_t szAppName[] = L"Monotone 2022 GameAcademy";

	WNDCLASSEXW wndclass;
	ZeroMemory(&wndclass, sizeof(wndclass));

	wndclass.cbSize = sizeof(WNDCLASSEXW);


	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WinProc::WndProc;
	wndclass.hInstance = hInstance;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;

	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = szAppName;

	// 윈도 클래스 등록
	if (!RegisterClassEx(&wndclass))
	{
		DWORD errormsg = GetLastError();
		LPWSTR output;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, errormsg, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (TCHAR*)&output, 0, nullptr);
		MessageBox(NULL, output, L"Error", MB_OK);
		return S_FALSE;
	}

	// 윈도 생성
	m_hWnd = CreateWindowW(
		szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, m_ClientWidth, m_ClientHeight,
		nullptr, nullptr, hInstance, nullptr);


	if (!m_hWnd)
	{
		DWORD errormsg = GetLastError();
		LPWSTR output;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, errormsg, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (TCHAR*)&output, 0, nullptr);
		MessageBox(NULL, output, L"Error", MB_OK);
		return S_FALSE;
	}

	m_GameEngine = GameEngine::GameEngineFactory::CreateOKEngine();
	m_GameEngine->Initialize((__int64)hInstance, (__int64)m_hWnd, m_ClientWidth, m_ClientHeight);

	// 생성된 윈도를 화면에 표시
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	//마우스 초기화
	m_tme.cbSize = sizeof(TRACKMOUSEEVENT);
	m_tme.dwFlags = TME_LEAVE;
	m_tme.hwndTrack = m_hWnd;
	m_tme.dwHoverTime = (DWORD)0.001f;

	m_hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

	return S_OK;
}

void WinProc::Finalize()
{
	m_GameEngine.reset();
	GameEngine::GameEngineFactory::ReleaseOKEngine();
}

LRESULT CALLBACK WinProc::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_Instance->MsgProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WinProc::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		{
			if (m_GameEngine != nullptr)
			{
				if (!(wParam == SIZE_MINIMIZED)) {

					m_ClientWidth = LOWORD(lParam);
					m_ClientHeight = HIWORD(lParam);

					m_GameEngine->OnResize(m_ClientWidth, m_ClientHeight);
				}
			}
		}
		break;
	case WM_MOUSEMOVE:
		if (m_GameEngine->GetMouseInWindow() == false)
		{
			m_GameEngine->SetMouseInWindow(true);
			m_tme.dwFlags = TME_LEAVE;
			TrackMouseEvent(&m_tme);
		}
		m_GameEngine->SetMousePos(lParam);
		break;
	case WM_MOUSEHOVER:
		m_GameEngine->SetMouseInWindow(true);
		m_tme.dwFlags = TME_LEAVE;
		TrackMouseEvent(&m_tme);
		// 마우스가 안으로 들어올때 이벤트를 발생시킨다.
		break;
	case WM_MOUSEWHEEL:
		m_GameEngine->SetMouseWheel(wParam);
		break;
	case WM_MOUSELEAVE:
		// 마우스가 윈도우 밖으로 나갈때 이벤트 발생			
		m_GameEngine->SetMouseInWindow(false);
		m_tme.dwFlags = TME_HOVER;
		TrackMouseEvent(&m_tme);
		break;
	case WM_ACTIVATE:
		// 윈도우가 시작할 때, 윈도우가 액티브됨을 인풋매니저에 알려줌
		m_GameEngine->SetMouseInWindow(LOWORD(wParam));
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
