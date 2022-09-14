#pragma once
#include "RenderPassBase.h"

class ViewBox;

/// <summary>
/// 
/// 포스트 프로세싱 패스
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

	//쉐이더의 버퍼 구성은 perFrame buffer와 perObjectBuffer로 이루어진다. 알아서 알잘딱 데이터를 넣어준다.
	//렌더하는데 필요한 박스(버텍스정보)
	static std::shared_ptr<ViewBox> m_ViewBox;
	static std::shared_ptr<VertexShader> m_VertexShader;
	std::shared_ptr<DepthStencilView> m_DepthStencilView;

	//RenderPass 쉐이더 종류의 이름 / 그에따른 쉐이더
	 std::shared_ptr<PixelShader> m_PixelShader;
};

