#pragma once

enum class eKeyState
{
	DOWN, //�� ���� ����
	UP, // �� ����
	STAY, // ������ ��
	NONE // �ƹ��͵� �ƴ�
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

	// ������ Ȱ��ȭ ���� Input �Ÿ���
	static bool bMouseInWindow;
	static bool bWindowActivated;

private:
	static std::vector<Keyinfo> m_KeyInfos;

	// ���콺 ��ǥ
	static POINTS m_cursorXY;
	static int m_Mousewheel;

};