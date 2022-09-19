#pragma once
#include "RenderPassBase.h"

/// <summary>
/// 
/// 오브젝트를 렌더하는 패스.
/// 단순 렌더패스와 쉐도우도 여기에 들어갈 것 같다.
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


	//쉐이더의 버퍼 구성은 perFrame buffer와 perObjectBuffer로 이루어진다. 알아서 알잘딱 데이터를 넣어준다.
private:
	//RenderPass 쉐이더 종류의 이름 / 그에따른 쉐이더
	std::unordered_map<std::wstring, ShaderSet> m_ShaderSet;
	std::shared_ptr<DepthStencilView> m_DepthStencilView;
};

