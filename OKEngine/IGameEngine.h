#pragma once
#include "dllDefine.h"


class OKENGINE_API IGameEngine
{
public:
	//���� �ʱ�ȭ
	virtual bool Initialize(long instance, long handle, int ScreenWidth, int ScreenHeight) abstract;

	//ȭ��� ��ȯ
	virtual void OnResize(int Width, int Height) abstract;

	/// ���콺�Է�
	virtual void SetMousePos(LPARAM lParam) abstract;
	virtual void SetMouseWheel(WPARAM lParam) abstract;

	virtual bool GetMouseInWindow() abstract;
	virtual void SetMouseInWindow(bool flag) abstract;

	virtual void Loop() abstract;
	virtual void Finalize() abstract;
};

