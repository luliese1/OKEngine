#pragma once
#include "RenderPassBase.h"

/// <summary>
/// 
/// ������Ʈ�� �����ϴ� �н�.
/// �ܼ� �����н��� �����쵵 ���⿡ �� �� ����.
/// 
/// </summary>

class DepthStencilView;

class RenderPass : public PassBase
{
public:
	RenderPass();

public:
	virtual void Initialize(ScreenInfo& sinfo, const GRAPHICSENGINE_PASS_DESC& passDesc, std::shared_ptr<Device> device) override;

	virtual void Begin( std::shared_ptr<Device> device, std::shared_ptr<DepthStencilView> dsv) override;
	virtual ShaderSet GetShaderSet(const std::wstring& shaderName) override;
	virtual void OnResize(ScreenInfo& sinfo, std::shared_ptr<Device> device) override;
	virtual void End(std::shared_ptr<Device> device) override;

	virtual void Finalize() override;

	virtual void UpdatePerFrameBuffer() override;
	virtual void UpdatePerObjectBuffer() override;

	virtual std::shared_ptr<DepthStencilView> GetDepthStencilView() const { return m_DepthStencilView; };
public:


	//���̴��� ���� ������ perFrame buffer�� perObjectBuffer�� �̷������. �˾Ƽ� ���ߵ� �����͸� �־��ش�.
private:
	//RenderPass ���̴� ������ �̸� / �׿����� ���̴�
	std::unordered_map<std::wstring, ShaderSet> m_ShaderSet;
	std::shared_ptr<DepthStencilView> m_DepthStencilView;
};

