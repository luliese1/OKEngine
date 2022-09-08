#pragma once

class VertexShader;
class PixelShader;
class RenderTargetView;
class DepthStencilView;
class Device;

struct ShaderSet
{
	std::shared_ptr<VertexShader> m_VertexShader;
	std::shared_ptr<PixelShader> m_PixelShader;
};

enum class ePassType
{
	RenderPass,
	PostProcess,
	INVALID
};

class PassBase
{
public:
	PassBase(ePassType passType);
	virtual ~PassBase() = default;

public:
	virtual void Initialize(ScreenInfo& sinfo, const GRAPHICSENGINE_PASS_DESC& passDesc, const std::shared_ptr<Device> device) abstract;

	virtual void Begin(std::shared_ptr<Device> device, std::shared_ptr<DepthStencilView> dsv) abstract;
	virtual ShaderSet GetShaderSet(const std::wstring& shaderName) abstract;
	virtual void OnResize(ScreenInfo& sinfo, std::shared_ptr<Device> device);
	virtual void End(std::shared_ptr<Device> device) abstract;

	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView(UINT idx);
	ComPtr<ID3D11RenderTargetView> GetRenderTargetView(UINT idx);

	std::vector<GRAPHICSENGINE_RENDER_TARGET_OUTPUT_LAYOUT> GetRenderTargetInfo() const { return m_RenderTargetInfo; }

	virtual void UpdatePerFrameBuffer() abstract;
	virtual void UpdatePerObjectBuffer() abstract;

	virtual void Finalize() abstract;

public:
	const ePassType GetPassType() const { return m_PassType; }

protected:
	virtual void SetRenderTargetViewListForBinding();

	std::wstring m_PassName;
	const ePassType m_PassType;
	UINT m_RenderTargetTextureCnt;
	ScreenInfo m_TextureSizeInfo;

	std::vector<GRAPHICSENGINE_RENDER_TARGET_OUTPUT_LAYOUT> m_RenderTargetInfo;

	//RenderTargetViews... 
	//나중에 MRT할때를 대비하여 RTT를 만들어둔다.
	std::vector<std::shared_ptr<RenderTargetView>> m_RenderTargetTexture;
	std::vector<ComPtr<ID3D11RenderTargetView>> m_BindingRenderTargetTexture;

private:
	PassBase() = default;
};

