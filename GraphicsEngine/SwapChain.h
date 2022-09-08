#pragma once

class Device;

class SwapChain
{
public:
	void Initialize(HWND hWnd, DXGI_FORMAT format, std::shared_ptr<Device> device, ScreenInfo& Sinfo);

	ComPtr<ID3D11RenderTargetView> GetBackBufferRenderTarget() {return m_pRenderTargetView;}

	void Finalize();
	void OnResize(ScreenInfo& Sinfo, DXGI_FORMAT format, std::shared_ptr<Device> device);
	void Present();

private:
	ComPtr<IDXGISwapChain> m_SwapChain;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
};

