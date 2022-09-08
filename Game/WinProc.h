#pragma once
#include "IGameEngine.h"
#include "GameEngineFactory.h"

class WinProc
{
public:
	WinProc();
	~WinProc();

	//루프문
	void Loop();
	//게임의 루프
	inline void GameLoop();

	//초기화
	HRESULT Initialize(HINSTANCE hInstance);
	//종료
	void Finalize();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static WinProc* m_Instance;
	std::shared_ptr<IGameEngine> m_GameEngine;

	HWND m_hWnd;
	HACCEL m_hAccelTable;
	MSG m_Msg;

	// 마우스 추적
	TRACKMOUSEEVENT m_tme;

	int m_ClientWidth;
	int m_ClientHeight;


};