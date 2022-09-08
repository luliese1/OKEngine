#pragma once
#include "dllDefine.h"


class OKENGINE_API IGameEngine
{
public:
	//엔진 초기화
	virtual bool Initialize(long instance, long handle, int ScreenWidth, int ScreenHeight) abstract;

	//화면비 변환
	virtual void OnResize(int Width, int Height) abstract;

	/// 마우스입력
	virtual void SetMousePos(LPARAM lParam) abstract;
	virtual void SetMouseWheel(WPARAM lParam) abstract;

	virtual bool GetMouseInWindow() abstract;
	virtual void SetMouseInWindow(bool flag) abstract;

	virtual void Loop() abstract;
	virtual void Finalize() abstract;
};

