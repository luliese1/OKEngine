#include "EnginePCH.h"
#include "Input.h"

int g_arrVK[(int)Key::LAST] =
{
	VK_LBUTTON,
	VK_RBUTTON,
	VK_RETURN,
	VK_SPACE,
	VK_CONTROL,
	VK_SHIFT,
	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,
	0x51,//Q
	0x57,//W
	0x45,//E
	0x52,//R
	0x41,//A
	0x53,//S
	0x44,//D
	0x46, //F
	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,
	VK_TAB,
	VK_CAPITAL
};

std::vector<Input::Keyinfo> Input::m_KeyInfos;
POINTS Input::m_cursorXY;
int Input::m_Mousewheel;
bool Input::bMouseInWindow;
bool Input::bWindowActivated;

/// <summary>
/// Bitmask 초기화, 마우스 좌표 초기화
/// </summary>


void Input::Init()
{
	m_cursorXY = {};
	bMouseInWindow = false;

	for (int i = 0; i < (int)Key::LAST; i++)
	{
		m_KeyInfos.push_back(Keyinfo(eKeyState::NONE, false));
	}
}

void Input::Finalize()
{
	m_KeyInfos.clear();
	m_KeyInfos.shrink_to_fit(); // 메모리 블럭 줄이기
}

/// <summary>
/// 키 입력을 확인하는 함수
/// </summary>
bool Input::KeyCheck(Key input, eKeyState state)
{
	if (m_KeyInfos[(int)input].m_keyState == state)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Input::MouseWheelCheck()
{
	if (bMouseInWindow == false || bWindowActivated == false)
	{
		return 0;
	}
	else
	{
		int result = m_Mousewheel / 120;
		return result;
	}

}

void Input::Tick()
{
	//if (bMouseInWindow == false || bWindowActivated == false)
	//{
	//	for (int i = 0; i < (int)Key::LAST; i++)
	//	{
	//		if (m_KeyInfos[i].m_keyState == eKeyState::STAY || m_KeyInfos[i].m_keyState == eKeyState::DOWN) {
	//			m_KeyInfos[i].m_keyState = eKeyState::UP;
	//		}
	//		else if (m_KeyInfos[i].m_keyState == eKeyState::UP)
	//		{
	//			m_KeyInfos[i].m_keyState = eKeyState::NONE;
	//		}
	//		m_KeyInfos[i].m_Prev = false;
	//	}
	//}
	//else
	{
		for (int i = 0; i < (int)Key::LAST; i++)
		{
			//이번 프레임에 충돌을 하였는가?
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				//전프레임에는?
				if (m_KeyInfos[i].m_Prev)
				{
					m_KeyInfos[i].m_keyState = eKeyState::STAY;
				}
				else
				{
					m_KeyInfos[i].m_keyState = eKeyState::DOWN;
				}
				m_KeyInfos[i].m_Prev = true;
			}
			else
			{
				if (m_KeyInfos[i].m_Prev)
				{
					m_KeyInfos[i].m_keyState = eKeyState::UP;
				}
				else
				{
					m_KeyInfos[i].m_keyState = eKeyState::NONE;
				}
				m_KeyInfos[i].m_Prev = false;
			}
		}
	}
}

/// <summary>
/// 마우스 좌표 설정하는 함수
/// </summary>
void Input::SetMousePos(LPARAM lParam)
{
	m_cursorXY = MAKEPOINTS(lParam);
}
void Input::SetMouseWheel(WPARAM wParam)
{
	m_Mousewheel = GET_WHEEL_DELTA_WPARAM(wParam);
}
POINTS& Input::GetMousePos()
{
	return m_cursorXY;
}

