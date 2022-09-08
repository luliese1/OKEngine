#pragma once
#include "IGameEngine.h"
#include "IGraphicsEngine.h"
#include "Input.h"
#include "Time.h"
#include "SceneManager.h"
#include "ModelLoader.h"

class OKEngine : public IGameEngine
{
public:
	//���� �ʱ�ȭ
	bool Initialize(long instance, long handle, int ScreenWidth, int ScreenHeight) override;

	//ȭ��� ��ȯ
	void OnResize(int Width, int Height) override;

	void SetMousePos(LPARAM lParam) override;
	void SetMouseWheel(WPARAM lParam) override;

	bool GetMouseInWindow() override;
	void SetMouseInWindow(bool flag) override;

	void Loop() override;
	void Finalize() override;

private:
	std::shared_ptr<IGraphicsEngine> m_GraphicsEngine;
	SceneManager m_SceneManager;
};

