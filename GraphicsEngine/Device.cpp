#include "pch.h"
#include "Device.h"

void Device::Initialize()
{
	HRESULT hr = S_FALSE;

	//D3D 버전 레벨
	D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL FeatureLevelOut;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HR(hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		0, 0,
		D3D11_SDK_VERSION,
		m_pD3DDevice.GetAddressOf(),
		&FeatureLevelOut,
		m_pD3DContext.GetAddressOf()
	));
}

void Device::DebugObject()
{
	ComPtr<ID3D11Debug> Debug;
	 
	m_pD3DDevice->QueryInterface(Debug.GetAddressOf());

	m_pD3DDevice->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Device") - 1, "Device");
	m_pD3DContext->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("DeviceContext") - 1, "DeviceContext");

	OutputDebugStringA("-------누수 오브젝트 목록입니다--------\r\n");
	Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	OutputDebugStringA("-------------------------------------\r\n"); 


}

void Device::Flush()
{
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_pD3DContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);

	ID3D11ShaderResourceView* nullSRVViews[8] = { nullptr, };

	m_pD3DContext->VSSetShaderResources(0, _countof(nullViews), nullSRVViews);
	m_pD3DContext->PSSetShaderResources(0, _countof(nullViews), nullSRVViews);

	ID3D11VertexShader* nullVS = { nullptr };
	ID3D11PixelShader* nullPS = { nullptr };
	m_pD3DContext->VSSetShader(nullVS, nullptr, 0);
	m_pD3DContext->PSSetShader(nullPS, nullptr, 0);

	m_pD3DContext->Flush();
}

void Device::Finalize()
{
	Flush();

#if defined(DEBUG) || defined(_DEBUG)  
	DebugObject();
#endif // DEBUG
}
