#include "pch.h"
#include "RenderTargetView.h"
#include "Device.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

RenderTargetView::RenderTargetView()
{

}

RenderTargetView::~RenderTargetView()
{

}

void RenderTargetView::Initialize(ScreenInfo& SInfo, std::shared_ptr<Device> device, GRAPHICSENGINE_RENDER_TARGET_OUTPUT_LAYOUT& renderTargetTextureDesc)
{
	m_MSAAFlag = SInfo.m_4xMsaaFlag;

	m_TextureDesc = renderTargetTextureDesc.m_TextureDesc;

	ComPtr<ID3D11Device> _device = device->GetDevice();

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	textureDesc.Width = SInfo.m_ScreenWidth;
	textureDesc.Height = SInfo.m_ScreenHeight;
	textureDesc.MipLevels = m_TextureDesc.m_MipLevels;
	textureDesc.ArraySize = m_TextureDesc.m_ArraySize; //텍스쳐 배열의 수
	textureDesc.Format = (DXGI_FORMAT)m_TextureDesc.m_Format;
	textureDesc.Usage = (D3D11_USAGE)m_TextureDesc.m_Usage;
	//렌더타겟과 쉐이더리소스 뷰로 바인드한다.
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = m_TextureDesc.m_CPUAccessFlags; //CPU에서 액세스를 할일이 있을때 추가하자. 
	textureDesc.MiscFlags = m_TextureDesc.m_MiscFlags; // 특정한 리소스에게 줄 FLAG들.. 

	if (m_MSAAFlag)
	{
		textureDesc.SampleDesc.Count = 4;
		textureDesc.SampleDesc.Quality = SInfo.m_4xMsaaQuality - 1;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	}

	HR(_device->CreateTexture2D(&textureDesc, NULL, m_Texture.GetAddressOf()));

	rtvDesc.Format = textureDesc.Format;
	rtvDesc.Texture2D.MipSlice = 0;

	srvDesc.Format = textureDesc.Format;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	//각각의 뷰 생성
	HR(_device->CreateRenderTargetView(m_Texture.Get(), &rtvDesc, m_RTV.GetAddressOf()));
	HR(_device->CreateShaderResourceView(m_Texture.Get(), &srvDesc, m_SRV.GetAddressOf()));

	if (m_MSAAFlag)
	{
		//single-sampled 생성
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//singlesample로
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;

		HR(_device->CreateTexture2D(&textureDesc, NULL, m_SingleSampledTexture.GetAddressOf()));
		HR(_device->CreateRenderTargetView(m_SingleSampledTexture.Get(), &rtvDesc, m_SingleSampledRTV.GetAddressOf()));
		HR(_device->CreateShaderResourceView(m_SingleSampledTexture.Get(), &srvDesc, m_SingleSampledSRV.GetAddressOf()));
	}
}

void RenderTargetView::OnResize(DXGI_FORMAT format, ScreenInfo& SInfo, std::shared_ptr<Device> device)
{
	m_Texture->Release();
	m_SRV->Release();
	m_RTV->Release();
	//m_SingleSampledTexture->Release();
	//m_SingleSampledSRV->Release();
	//m_SingleSampledRTV->Release();

	m_MSAAFlag = SInfo.m_4xMsaaFlag;

	ComPtr<ID3D11Device> _device = device->GetDevice();

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	textureDesc.Width = SInfo.m_ScreenWidth;
	textureDesc.Height = SInfo.m_ScreenHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1; //텍스쳐 배열의 수
	textureDesc.Format = format;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	//렌더타겟과 쉐이더리소스 뷰로 바인드한다.
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0; //CPU에서 액세스를 할일이 있을때 추가하자. 
	textureDesc.MiscFlags = 0; // 특정한 리소스에게 줄 FLAG들.. 

	if (m_MSAAFlag)
	{
		textureDesc.SampleDesc.Count = 4;
		textureDesc.SampleDesc.Quality = SInfo.m_4xMsaaQuality - 1;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	}

	HR(_device->CreateTexture2D(&textureDesc, NULL, m_Texture.GetAddressOf()));

	rtvDesc.Format = textureDesc.Format;
	rtvDesc.Texture2D.MipSlice = 0;


	srvDesc.Format = textureDesc.Format;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	//각각의 뷰 생성
	HR(_device->CreateRenderTargetView(m_Texture.Get(), &rtvDesc, m_RTV.GetAddressOf()));
	HR(_device->CreateShaderResourceView(m_Texture.Get(), &srvDesc, m_SRV.GetAddressOf()));

	if (m_MSAAFlag)
	{
		//single-sampled 생성
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//singlesample로
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;

		HR(_device->CreateTexture2D(&textureDesc, NULL, m_SingleSampledTexture.GetAddressOf()));
		HR(_device->CreateRenderTargetView(m_SingleSampledTexture.Get(), &rtvDesc, m_SingleSampledRTV.GetAddressOf()));
		HR(_device->CreateShaderResourceView(m_SingleSampledTexture.Get(), &srvDesc, m_SingleSampledSRV.GetAddressOf()));
	}
}

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> RenderTargetView::GetShaderResourceView()
{
	if (m_MSAAFlag)
	{
		return m_SingleSampledSRV;
	}
	else
	{
		return m_SRV;
	}
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView::GetRenderTargetView()
{
	return m_RTV;
}

void RenderTargetView::Finalize()
{
}

void RenderTargetView::ResolveSubresource(std::shared_ptr<Device> device)
{
	device->GetContext()->ResolveSubresource(m_SingleSampledTexture.Get(), 0, m_Texture.Get(), 0, DXGI_FORMAT_R8G8B8A8_UNORM);
}

/////////////////////////////////////


void ViewBox::Initialize(std::shared_ptr<Device> device)
{
	ViewBoxVertex Boxvertex[4] =
	{
		{{-1.f,+1.f,+0.f}, {0.f, 0.f}},
		{{+1.f,+1.f,+0.f}, {1.f, 0.f}},
		{{-1.f,-1.f,+0.f}, {0.f, 1.f}},
		{{+1.f,-1.f,+0.f}, {1.f, 1.f}}
	};

	UINT index[6] =
	{
		0,2,1, 3,1,2
	};

	m_VertexBuffer= std::make_shared<VertexBuffer>();
	m_IndexBuffer = std::make_shared<IndexBuffer>();

	m_VertexBuffer->Initialize((void*)Boxvertex, sizeof(ViewBoxVertex), 4, device);
	m_IndexBuffer->Initialize((void*)index, 6, device);

}

void ViewBox::OnResize(std::shared_ptr<Device> device)
{
}

void ViewBox::Finalize()
{
	m_VertexBuffer.reset();
	m_VertexBuffer.reset();
}
