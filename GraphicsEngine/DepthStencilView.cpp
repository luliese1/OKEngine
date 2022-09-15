#include "pch.h"
#include "DepthStencilView.h"
#include "Device.h"



void DepthStencilView::Initialize(std::shared_ptr<Device> device, ScreenInfo& SInfo)
{
	//뎁스스텐실 버퍼와 뷰를 생성한다.

	OnResize(SInfo, DXGI_FORMAT_D24_UNORM_S8_UINT, device);
	
	D3D11_DEPTH_STENCIL_DESC depthstencilStateDesc;
	//DepthStencilState
	if (m_DepthStencilState == nullptr)
	{
		ZeroMemory(&depthstencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		depthstencilStateDesc.DepthEnable = true;
		depthstencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthstencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthstencilStateDesc.StencilEnable = true;
		depthstencilStateDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthstencilStateDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		// Stencil operations if pixel is front-facing. 
		depthstencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthstencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthstencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthstencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		// Stencil operations if pixel is back-facing. 
		depthstencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthstencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthstencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthstencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HR(device->GetDevice()->CreateDepthStencilState(&depthstencilStateDesc, &m_DepthStencilState));
	}

	if (m_DepthDisableStencilState == nullptr)
	{
		//뎁스 끈 모드
		depthstencilStateDesc.DepthEnable = false;
		depthstencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthstencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthstencilStateDesc.StencilEnable = true;
		depthstencilStateDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthstencilStateDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		// Stencil operations if pixel is front-facing. 
		depthstencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthstencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthstencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthstencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		// Stencil operations if pixel is back-facing. 
		depthstencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthstencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthstencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthstencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HR(device->GetDevice()->CreateDepthStencilState(&depthstencilStateDesc, &m_DepthDisableStencilState));
	}


}

void DepthStencilView::Finalize()
{
}

void DepthStencilView::OnResize(ScreenInfo& Sinfo, DXGI_FORMAT format, std::shared_ptr<Device> device)
{
	//뎁스스텐실 버퍼와 뷰를 생성한다.
	m_DepthStencilBuffer.ReleaseAndGetAddressOf();
	//m_DepthStencilSRV.ReleaseAndGetAddressOf();
	m_DepthStencilView.ReleaseAndGetAddressOf();

	ComPtr<ID3D11Device> _device = device->GetDevice();

	//뎁스스텐실 텍스쳐에 대한 정보를 담은 구조체
	D3D11_TEXTURE2D_DESC DepthBufferDesc;
	ZeroMemory(&DepthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	DepthBufferDesc.Width = Sinfo.m_ScreenWidth; // 크기
	DepthBufferDesc.Height = Sinfo.m_ScreenHeight;
	DepthBufferDesc.MipLevels = 1; //밉맵 수준의 개수, 밉맵이란..?
	DepthBufferDesc.ArraySize = 1; // 텍스처 배열의 텍스처 개수. 깊이스텐실 버퍼에는 하나만 필요
	DepthBufferDesc.Format = format; //텍셀의 형식?

	DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT; //텍스처의 용도. 자원을 GPU가 읽고 써야한다면 Default. CPU는 읽거나 쓸 수 없음\
	//이외에도 IMMUTABLE, DYNAMIC, STAGING... 이 있다
	DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // 자원을 파이프라인에 어떤식으로 묶을 것인가? 
	DepthBufferDesc.CPUAccessFlags = 0; //CPU가 접근하는 방식. 깊이스텐실은 GPU만 읽고 쓴다. 그러므로 0
	DepthBufferDesc.MiscFlags = 0; //기타 플래그

	//뎁스스텐실의 텍스쳐의 쉐이더리소스 정보를 담은 구조체
	D3D11_SHADER_RESOURCE_VIEW_DESC DepthBufferSRVDesc;
	ZeroMemory(&DepthBufferSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	DepthBufferSRVDesc.Format = DepthBufferDesc.Format;
	DepthBufferSRVDesc.Texture2D.MipLevels = 0; // 크기
	DepthBufferSRVDesc.Texture2D.MostDetailedMip = 0;


	//4X MSAA를 사용할때는 스왑체인의 설정과 동일하게.
	// 4X MSAA를 사용하려면 ? => 스왑 체인에 대한 MSAA 매개 변수를 설정해줍니다.
	if (Sinfo.m_4xMsaaFlag)
	{
		DepthBufferDesc.SampleDesc.Count = 4;
		DepthBufferDesc.SampleDesc.Quality = Sinfo.m_4xMsaaQuality - 1;
	}
	else
	{
		DepthBufferDesc.SampleDesc.Count = 1;
		DepthBufferDesc.SampleDesc.Quality = 0;
	}

	HR(_device->CreateTexture2D(&DepthBufferDesc, 0, &m_DepthStencilBuffer));

	// DepthStencilView를 만들 때 사용할 뎁스 스텐실 뷰 데스크라이브를 작성하고 매개변수로 던진다.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR(_device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &depthStencilViewDesc, &m_DepthStencilView));
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilView::m_DepthStencilState;
Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilView::m_DepthDisableStencilState;

