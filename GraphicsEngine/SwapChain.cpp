#include "pch.h"
#include "SwapChain.h"
#include "Device.h"

void SwapChain::Initialize(HWND hWnd, DXGI_FORMAT format, std::shared_ptr<Device> device, ScreenInfo& SInfo)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	{
		ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		//�ĸ� ������ �Ӽ���
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //�� ������ �뵵�� �����ΰ�? (�ĸ� ���ۿ� �������� �ϴ� �뵵(����ü��))
		SwapChainDesc.BufferCount = 1;  //����ü�ο� ���� ������ ���� �Ϲ������� �ĸ�����ϳ�(���߹��۸�), ���߹��۸��� ������ �� �� �ִ�.
		SwapChainDesc.OutputWindow = hWnd; // ������ ����� ǥ���� â�� �ڵ�
		SwapChainDesc.Windowed = true; // â���ȿ��
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		SwapChainDesc.Flags = 0;

		//��������Ʈ�� �÷��׵��� �߰� ������ �� �ִ�. ���⿣ ���Ե��� ����.
		SwapChainDesc.BufferDesc.Width = SInfo.m_ScreenWidth; // ��ũ����ũ��
		SwapChainDesc.BufferDesc.Height = SInfo.m_ScreenHeight;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60; //���÷��� ��� ������
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1; 
		SwapChainDesc.BufferDesc.Format = format; //�ĸ���� �ȼ� ����
		SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		//�̿ܿ��� ��ĵ����, ��ʸ�带 ������ �� ����
		//����ǥ��ȭ�� ���� �༮. ǥ�� ������ ǰ��

		//https://stackoverflow.com/questions/40275577/how-to-sample-a-srv-when-enable-msaa-x4directx11
		//https://stackoverflow.com/questions/13238723/multisampling-msaa-for-directx11-directx10-with-d3dimage-shared-resource
		//https://stackoverflow.com/questions/10018230/multisampling-and-direct3d10-d3dimage-interop
		// Use 4X MSAA? 
		if (SInfo.m_4xMsaaFlag)
		{
			SwapChainDesc.SampleDesc.Count = 4;
			SwapChainDesc.SampleDesc.Quality = SInfo.m_4xMsaaQuality - 1;
		}
		// No MSAA
		else
		{
			SwapChainDesc.SampleDesc.Count = 1;
			SwapChainDesc.SampleDesc.Quality = 0;
		}
	}

	//����͸� �޾ƿͼ� ����ü���� ���� ������ټ��� �ִ�.
	IDXGIFactory* dxgiDevice = nullptr;
	HR(device->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
	IDXGIAdapter* dxgiAdapter = nullptr;
	HR(FAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter)));
	IDXGIFactory* dxgiFactory = nullptr;
	HR(FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory)));

	dxgiFactory->CreateSwapChain(device->GetDevice().Get(), &SwapChainDesc, m_SwapChain.GetAddressOf());

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	OnResize(SInfo, format, device);
}

void SwapChain::Finalize()
{
}

void SwapChain::OnResize(ScreenInfo& Sinfo, DXGI_FORMAT format, std::shared_ptr<Device> device)
{
	device->Flush();

	m_pRenderTargetView.ReleaseAndGetAddressOf();

	//����Ÿ�� ���� ����
	ID3D11Texture2D* backBuffer = nullptr;
	//����ü���� ����Ű�� �����͸� ����
	//ù �Ű������� ������ϴ� �ĸ� ������ ����(������ �ִ� ��� �߿���) ��°�� ������ �������̽�
	HR(m_SwapChain->ResizeBuffers(1, Sinfo.m_ScreenWidth, Sinfo.m_ScreenHeight, format, 0));
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));

	//ù �Ű������� ���� ������� ����� �ڿ�. ��� ���� �ĸ���ۿ� ���� �����͸� �־��ش�.
	//��°�� �ڿ��� ��� ���ҵ��� �ڷ�����?(���ؾȵ�
	HR(device->GetDevice()->CreateRenderTargetView(backBuffer, NULL, &m_pRenderTargetView));

	//�ĸ� ���۸� ����ϰ� ���� ����������. COM ��� �����͵��� ����ϰ� ��ġ�� ����������Ѵ�.
	ReleaseCOM(backBuffer);
}

void SwapChain::Present()
{
	m_SwapChain->Present(0, 0);
}
