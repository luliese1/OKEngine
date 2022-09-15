#include "pch.h"
#include "DepthStencilView.h"
#include "Device.h"



void DepthStencilView::Initialize(std::shared_ptr<Device> device, ScreenInfo& SInfo)
{
	//�������ٽ� ���ۿ� �並 �����Ѵ�.

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
		//���� �� ���
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
	//�������ٽ� ���ۿ� �並 �����Ѵ�.
	m_DepthStencilBuffer.ReleaseAndGetAddressOf();
	//m_DepthStencilSRV.ReleaseAndGetAddressOf();
	m_DepthStencilView.ReleaseAndGetAddressOf();

	ComPtr<ID3D11Device> _device = device->GetDevice();

	//�������ٽ� �ؽ��Ŀ� ���� ������ ���� ����ü
	D3D11_TEXTURE2D_DESC DepthBufferDesc;
	ZeroMemory(&DepthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	DepthBufferDesc.Width = Sinfo.m_ScreenWidth; // ũ��
	DepthBufferDesc.Height = Sinfo.m_ScreenHeight;
	DepthBufferDesc.MipLevels = 1; //�Ӹ� ������ ����, �Ӹ��̶�..?
	DepthBufferDesc.ArraySize = 1; // �ؽ�ó �迭�� �ؽ�ó ����. ���̽��ٽ� ���ۿ��� �ϳ��� �ʿ�
	DepthBufferDesc.Format = format; //�ؼ��� ����?

	DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT; //�ؽ�ó�� �뵵. �ڿ��� GPU�� �а� ����Ѵٸ� Default. CPU�� �аų� �� �� ����\
	//�̿ܿ��� IMMUTABLE, DYNAMIC, STAGING... �� �ִ�
	DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // �ڿ��� ���������ο� ������� ���� ���ΰ�? 
	DepthBufferDesc.CPUAccessFlags = 0; //CPU�� �����ϴ� ���. ���̽��ٽ��� GPU�� �а� ����. �׷��Ƿ� 0
	DepthBufferDesc.MiscFlags = 0; //��Ÿ �÷���

	//�������ٽ��� �ؽ����� ���̴����ҽ� ������ ���� ����ü
	D3D11_SHADER_RESOURCE_VIEW_DESC DepthBufferSRVDesc;
	ZeroMemory(&DepthBufferSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	DepthBufferSRVDesc.Format = DepthBufferDesc.Format;
	DepthBufferSRVDesc.Texture2D.MipLevels = 0; // ũ��
	DepthBufferSRVDesc.Texture2D.MostDetailedMip = 0;


	//4X MSAA�� ����Ҷ��� ����ü���� ������ �����ϰ�.
	// 4X MSAA�� ����Ϸ��� ? => ���� ü�ο� ���� MSAA �Ű� ������ �������ݴϴ�.
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

	// DepthStencilView�� ���� �� ����� ���� ���ٽ� �� ����ũ���̺긦 �ۼ��ϰ� �Ű������� ������.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR(_device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &depthStencilViewDesc, &m_DepthStencilView));
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilView::m_DepthStencilState;
Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilView::m_DepthDisableStencilState;

