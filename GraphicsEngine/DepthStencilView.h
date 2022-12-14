#pragma once

class Device;

class DepthStencilView
{
public:
	void Initialize(std::shared_ptr<Device> device, ScreenInfo& SInfo,const GRAPHICSENGINE_DEPTH_STENCIL_BUFFER_LAYOUT& depthStencilDesc);

	ComPtr<ID3D11DepthStencilView> GetDepthStencilView(){ return m_DepthStencilView; }
	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return m_DepthStencilSRV; }
	ComPtr<ID3D11DepthStencilState> GetDepthStencilState(){ return m_DepthStencilState;}
	ComPtr<ID3D11DepthStencilState> GetDisableDepthStencilState() { return m_DepthDisableStencilState; }

	void Finalize();
	void OnResize(ScreenInfo& Sinfo, DXGI_FORMAT format, std::shared_ptr<Device> device);

private:
	GRAPHICSENGINE_DEPTH_STENCIL_BUFFER_LAYOUT m_DepthStencilDesc;

	ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	ComPtr<ID3D11ShaderResourceView> m_DepthStencilSRV;
	ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;

	static ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
	static ComPtr<ID3D11DepthStencilState> m_DepthDisableStencilState;
};

