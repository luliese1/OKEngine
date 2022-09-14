#pragma once

class Device;

class DepthStencilView
{
public:
	void Initialize(std::shared_ptr<Device> device, ScreenInfo& SInfo);

	ComPtr<ID3D11DepthStencilView> GetDepthStencilView(){ return m_DepthStencilView; }
	ComPtr<ID3D11DepthStencilState> GetDepthStencilState(){ return m_DepthStencilState;}
	ComPtr<ID3D11DepthStencilState> GetDisableDepthStencilState() { return m_DepthDisableStencilState; }

	void Finalize();
	void OnResize(ScreenInfo& Sinfo, DXGI_FORMAT format, std::shared_ptr<Device> device);

private:
	
	ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	//ComPtr<ID3D11ShaderResourceView> m_DepthStencilSRV;
	ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;

	static ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
	static ComPtr<ID3D11DepthStencilState> m_DepthDisableStencilState;
};

