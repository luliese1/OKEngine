#pragma once

enum class eKeyState
{
	DOWN, //막 누른 시점
	UP, // 뗀 시점
	STAY, // 누르는 중
	NONE // 아무것도 아님
};
enum class Key
{
	LEFTMOUSE,
	RIGHTMOUSE,
	ENTER,
	SPACE,
	CTRL,
	SHIFT,
	UPARROW,
	DOWNARROW,
	LEFTARROW,
	RIGHTARROW,
	Q,
	W,
	E,
	R,
	A,
	S,
	D,
	F,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	TAB,
	CAPS_LOCK,
	LAST
};

class Input
{
	struct Keyinfo
	{
		eKeyState m_keyState;
		bool m_Prev;

		Keyinfo(eKeyState state, bool prev) :m_keyState(state), m_Prev(prev) {};
	};

public:

	static void Init();
	static void Finalize();

	// Key Input
	static bool KeyCheck(Key input, eKeyState state);
	static int MouseWheelCheck();
	static void Tick();

	// Mouse Pos
	static void SetMousePos(LPARAM lParam);
	static void SetMouseWheel(WPARAM lParam);
	static void InitMouseWheel() { m_Mousewheel = 0; };
	static POINTS& GetMousePos();

	// 윈도우 활성화 관련 Input 거름망
	static bool bMouseInWindow;
	static bool bWindowActivated;

private:
	static std::vector<Keyinfo> m_KeyInfos;

	// 마우스 좌표
	static POINTS m_cursorXY;
	static int m_Mousewheel;

};