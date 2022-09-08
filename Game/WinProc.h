#pragma once
#include "IGameEngine.h"
#include "GameEngineFactory.h"

class WinProc
{
public:
	WinProc();
	~WinProc();

	//������
	void Loop();
	//������ ����
	inline void GameLoop();

	//�ʱ�ȭ
	HRESULT Initialize(HINSTANCE hInstance);
	//����
	void Finalize();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static WinProc* m_Instance;
	std::shared_ptr<IGameEngine> m_GameEngine;

	HWND m_hWnd;
	HACCEL m_hAccelTable;
	MSG m_Msg;

	// ���콺 ����
	TRACKMOUSEEVENT m_tme;

	int m_ClientWidth;
	int m_ClientHeight;


};