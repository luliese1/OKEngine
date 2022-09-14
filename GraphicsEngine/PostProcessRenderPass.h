#pragma once
#include "RenderPassBase.h"

class ViewBox;

/// <summary>
/// 
/// ����Ʈ ���μ��� �н�
/// 
/// </summary>

class PostProcessRenderPass : public PassBase
{
public:
	PostProcessRenderPass();

public:
	virtual void Initialize(ScreenInfo& sinfo, const GRAPHICSENGINE_PASS_DESC& passDesc, std::shared_ptr<Device> device) override;

	virtual void Begin(std::shared_ptr<Device> device, std::shared_ptr<DepthStencilView> dsv) override;
	virtual ShaderSet GetShaderSet(const std::wstring& shaderName) override;
	virtual void End(std::shared_ptr<Device> device) override;

	virtual void Finalize() override;

	virtual void UpdatePerFrameBuffer() override;
	virtual void UpdatePerObjectBuffer() override;

private:

	//���̴��� ���� ������ perFrame buffer�� perObjectBuffer�� �̷������. �˾Ƽ� ���ߵ� �����͸� �־��ش�.
	//�����ϴµ� �ʿ��� �ڽ�(���ؽ�����)
	static std::shared_ptr<ViewBox> m_ViewBox;
	static std::shared_ptr<VertexShader> m_VertexShader;
	std::shared_ptr<DepthStencilView> m_DepthStencilView;

	//RenderPass ���̴� ������ �̸� / �׿����� ���̴�
	 std::shared_ptr<PixelShader> m_PixelShader;
};

