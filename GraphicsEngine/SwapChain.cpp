#include "pch.h"
#include "SwapChain.h"
#include "Device.h"

void SwapChain::Initialize(HWND hWnd, DXGI_FORMAT format, std::shared_ptr<Device> device, ScreenInfo& SInfo)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	{
		ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		//후면 버퍼의 속성들
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //이 버퍼의 용도가 무엇인가? (후면 버퍼에 렌더링을 하는 용도(스왑체인))
		SwapChainDesc.BufferCount = 1;  //스왑체인에 사용될 버퍼의 개수 일반적으로 후면버퍼하나(이중버퍼링), 삼중버퍼링을 지정할 수 도 있다.
		SwapChainDesc.OutputWindow = hWnd; // 렌더링 결과를 표시할 창의 핸들
		SwapChainDesc.Windowed = true; // 창모드효과
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		SwapChainDesc.Flags = 0;

		//스왑이펙트와 플래그들을 추가 설정할 수 있다. 여기엔 포함되지 않음.
		SwapChainDesc.BufferDesc.Width = SInfo.m_ScreenWidth; // 스크린의크기
		SwapChainDesc.BufferDesc.Height = SInfo.m_ScreenHeight;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60; //디스플레이 모드 갱신율
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1; 
		SwapChainDesc.BufferDesc.Format = format; //후면버퍼 픽셀 형식
		SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		//이외에도 스캔라인, 비례모드를 설정할 수 있음
		//다중표본화를 위한 녀석. 표본 개수와 품질

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

	//어댑터를 받아와서 스왑체인을 따로 만들어줄수도 있다.
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

	//렌더타겟 뷰의 생성
	ID3D11Texture2D* backBuffer = nullptr;
	//스왑체인을 가리키는 포인터를 얻음
	//첫 매개변수는 얻고자하는 후면 버퍼의 색인(여러개 있는 경우 중요함) 둘째는 버퍼의 인터페이스
	HR(m_SwapChain->ResizeBuffers(1, Sinfo.m_ScreenWidth, Sinfo.m_ScreenHeight, format, 0));
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));

	//첫 매개변수는 렌더 대상으로 사용할 자원. 방금 얻은 후면버퍼에 대한 포인터를 넣어준다.
	//둘째는 자원이 담긴 원소들의 자료형식?(이해안됨
	HR(device->GetDevice()->CreateRenderTargetView(backBuffer, NULL, &m_pRenderTargetView));

	//후면 버퍼를 사용하고 나면 해제해주자. COM 기반 포인터들은 사용하고 마치면 해제해줘야한다.
	ReleaseCOM(backBuffer);
}

void SwapChain::Present()
{
	m_SwapChain->Present(0, 0);
}
