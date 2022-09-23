#include "pch.h"
#include "DXGraphicsEngine.h"
#include "SpriteBatch.h"

//Renderer
#include "Device.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "SwapChain.h"

//Manager
#include "DXResourceManager.h"
#include "RenderInfoManager.h"

//Resources
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "SamplerState.h"

//Shader
#include "RenderPass.h"
#include "ShaderBase.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "ConstantBuffer.h"
#include "TextureResourceBuffer.h"
#include "SamplerBuffer.h"


DXGraphicsEngine::DXGraphicsEngine() :
	m_hInst(nullptr), m_hWnd(nullptr), m_Device(std::make_shared<Device>()), m_SwapChain(std::make_shared<SwapChain>()), m_ResourceManager(std::make_shared<DXResourceManager>()), m_ScreenInfo(), m_RenderInfoManager(std::make_shared<RenderInfoManager>())
{
}

DXGraphicsEngine::~DXGraphicsEngine()
{
}

int DXGraphicsEngine::Initialize(GRAPHICSENGINE_ENGINE_DESC& GraphicsEngineDesc)
{
	m_hInst = (HINSTANCE)GraphicsEngineDesc.instance;
	m_hWnd = (HWND)GraphicsEngineDesc.handle;
	//m_Device = std::make_shared<Device>();

	m_ScreenInfo.m_ScreenWidth = GraphicsEngineDesc.ScreenWidth;
	m_ScreenInfo.m_ScreenHeight = GraphicsEngineDesc.ScreenHeight;
	m_ScreenInfo.m_4xMsaaFlag = false;
	m_ScreenInfo.m_4xMsaaQuality = 0;

	m_Device->Initialize();

	if (m_ScreenInfo.m_4xMsaaFlag)
	{
		m_Device->GetDevice()->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_ScreenInfo.m_4xMsaaQuality);
		assert(m_ScreenInfo.m_4xMsaaQuality > 0);
	}

	m_SwapChain->Initialize(m_hWnd, DXGI_FORMAT_R8G8B8A8_UNORM, m_Device, m_ScreenInfo);

	m_ResourceManager->Initialize(m_Device);

	//CreateRenderState();
	//CreateBasicPass(GraphicsEngineDesc.m_BasicPass);

	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(m_Device->GetContext().Get());

	return true;
}

void DXGraphicsEngine::OnResize(int Width, int Height)
{
	m_ScreenInfo.m_ScreenWidth = Width;
	m_ScreenInfo.m_ScreenHeight = Height;

	m_SwapChain->OnResize(m_ScreenInfo, DXGI_FORMAT_R8G8B8A8_UNORM, m_Device);

	D3D11_VIEWPORT ViewPort;

	ViewPort.TopLeftX = 0.0f;
	ViewPort.TopLeftY = 0.0f;
	ViewPort.Width = (float)m_ScreenInfo.m_ScreenWidth;
	ViewPort.Height = (float)m_ScreenInfo.m_ScreenHeight;
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;

	m_Device->GetContext()->RSSetViewports(1, &ViewPort);

	m_ResourceManager->OnResize(m_ScreenInfo);
}

void DXGraphicsEngine::StartRender()
{
	StartRender_Excute();
}
void DXGraphicsEngine::SetPerFrameConstantBuffer(void* ConstantBuffer)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	ComPtr<ID3D11DeviceContext> Context = m_Device->GetContext();

	//perFrame ConstantBuffer 밀어넣기
	auto perFrameBuffer = ShaderBase::GetPerFrameBuffer();
	if (perFrameBuffer != nullptr)
	{
		ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		Context->Map(perFrameBuffer->GetConstantBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
		memcpy(mappedSubresource.pData, ConstantBuffer, perFrameBuffer->GetBufferSize());
		Context->Unmap(perFrameBuffer->GetConstantBuffer().Get(), 0);

		Context->VSSetConstantBuffers(perFrameBuffer->GetRegisterSlot(), 1, perFrameBuffer->GetConstantBuffer().GetAddressOf());
		Context->PSSetConstantBuffers(perFrameBuffer->GetRegisterSlot(), 1, perFrameBuffer->GetConstantBuffer().GetAddressOf());
	}
}
void DXGraphicsEngine::Present()
{
	Present_Execute();
}

void DXGraphicsEngine::RenderObject(size_t MeshID, void* ConstantBuffer)
{
	Render_Execute(MeshID, ConstantBuffer);
}

void DXGraphicsEngine::RenderObjects(size_t* MeshID, void* ConstantBuffer, UINT ConstantBufSize, UINT objectCnt)
{
	for (UINT i = 0; i < objectCnt; i++)
	{
		Render_Execute(*(MeshID + i), ((char*)ConstantBuffer + i * ConstantBufSize));
	}
	m_BindingRenderPass->End(m_Device);
}

void DXGraphicsEngine::AddObject(size_t MeshID, MeshInfo meshInfo)
{
	m_RenderInfoManager->PushMeshInfo(MeshID, meshInfo);
}
void DXGraphicsEngine::DeleteObject(size_t MeshID)
{
	m_RenderInfoManager->DeleteMeshInfo(MeshID);
}

void DXGraphicsEngine::BindRenderPass(std::wstring passName)
{
	auto Foundpass = m_ResourceManager->GetPass(passName);
	m_Device->Flush();

	if (Foundpass == nullptr)
	{
		Assert("GraphicsEngine BindRenderPass : Pass Binding Failed ");
	}
	else
	{

		if (Foundpass->GetPassType() == ePassType::RenderPass)
		{
			m_BindingRenderPass = std::static_pointer_cast<RenderPass>(Foundpass);

			m_BindingRenderPass->Begin(m_Device, m_BindingRenderPass->GetDepthStencilView());

			auto PassTextureInfo = m_BindingRenderPass->GetTextureSizeInfo();
			SetViewport(PassTextureInfo.m_ScreenWidth, PassTextureInfo.m_ScreenHeight);

			std::vector<ComPtr<ID3D11RenderTargetView>> BindingRenderTargets;
			UINT backbufferCnt = 0;
			UINT passCnt = 0;
			UINT otherpassCnt = 0;

			for (auto& renderTargetInfo : m_BindingRenderPass->GetRenderTargetInfo())
			{
				switch (renderTargetInfo.m_ResourceSource)
				{
				case SHADER_RENDER_TARGET::BACKBUFFER:
					{
						BindingRenderTargets.push_back(m_SwapChain->GetBackBufferRenderTarget());
						backbufferCnt++;
					}
					break;
				case SHADER_RENDER_TARGET::PASS:
					{
						BindingRenderTargets.push_back(m_BindingRenderPass->GetRenderTargetView(passCnt));
						passCnt++;
					}
					break;
				default:
					break;
				}
			}

			if (BindingRenderTargets.size() > 0)
			{
				//RTTs 밀어넣기
				m_Device->GetContext()->OMSetRenderTargets(BindingRenderTargets.size(), BindingRenderTargets[0].GetAddressOf(), m_BindingRenderPass->GetDepthStencilView()->GetDepthStencilView().Get());
			}
			else
			{
				m_Device->GetContext()->OMSetRenderTargets(0, nullptr, m_BindingRenderPass->GetDepthStencilView()->GetDepthStencilView().Get());
			}

			auto rasterizerState = m_ResourceManager->GetRaterizerState(m_BindingRenderPass->GetRasterizerState());
			//래스터라이저 세팅
			m_Device->GetContext()->RSSetState(rasterizerState.Get());

		}
		else
		{
			Assert("GraphicsEngine BindRenderPass : Pass Binding Failed ");
		}
	}
}

void DXGraphicsEngine::BindPostProcessPass(std::wstring passName)
{
	m_BindingPostProcessPass = m_ResourceManager->GetPass(passName);
	m_Device->Flush();

	if (m_BindingPostProcessPass == nullptr)
	{
		Assert("GraphicsEngine BindPostProcessPass : Pass Binding Failed ");
	}
	else
	{
		if (m_BindingPostProcessPass->GetPassType() == ePassType::PostProcess)
		{
			m_BindingPostProcessPass->Begin(m_Device, nullptr);

			auto PassTextureInfo = m_BindingPostProcessPass->GetTextureSizeInfo();

			SetViewport(PassTextureInfo.m_ScreenWidth, PassTextureInfo.m_ScreenHeight);

			std::vector<ComPtr<ID3D11RenderTargetView>> BindingRenderTargets;
			UINT backbufferCnt = 0;
			UINT passCnt = 0;
			UINT otherpassCnt = 0;

			for (auto& renderTargetInfo : m_BindingPostProcessPass->GetRenderTargetInfo())
			{
				switch (renderTargetInfo.m_ResourceSource)
				{
				case SHADER_RENDER_TARGET::BACKBUFFER:
					{
						BindingRenderTargets.push_back(m_SwapChain->GetBackBufferRenderTarget());
						backbufferCnt++;
					}
					break;
				case SHADER_RENDER_TARGET::PASS:
					{
						BindingRenderTargets.push_back(m_BindingPostProcessPass->GetRenderTargetView(passCnt));
					}
					break;
				default:
					break;
				}
			}

			m_Device->GetContext()->OMSetRenderTargets(BindingRenderTargets.size(), BindingRenderTargets[0].GetAddressOf(), nullptr);

			auto rasterizerState = m_ResourceManager->GetRaterizerState(m_BindingPostProcessPass->GetRasterizerState());
			//래스터라이저 세팅
			m_Device->GetContext()->RSSetState(rasterizerState.Get());

		}
		else
		{
			Assert("GraphicsEngine BindRenderPass : Pass Binding Failed ");
		}
	}
}

void DXGraphicsEngine::SetPerObjectConstantBuffer(void* bufferSrc)
{
	auto context = m_Device->GetContext();
	auto shaderSet = m_BindingPostProcessPass->GetShaderSet(L"");

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	//VS데이터 밀어넣기
	{
		//ConstantBuffer
		auto vsConstantBuffers = shaderSet.m_VertexShader->GetConstantBuffer();
		for (const auto& Buffers : vsConstantBuffers)
		{
			ComPtr<ID3D11Buffer> ConstantBuffer = Buffers->GetConstantBuffer();

			context->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
			memcpy(mappedSubresource.pData, bufferSrc, Buffers->GetBufferSize());
			context->Unmap(ConstantBuffer.Get(), 0);

			context->VSSetConstantBuffers(Buffers->GetRegisterSlot(), 1, Buffers->GetConstantBuffer().GetAddressOf());
		}

		//Sampler
		auto vsSamplerBuff = shaderSet.m_VertexShader->GetSamplerBuffer();
		for (const auto& Buffers : vsSamplerBuff)
		{
			context->VSSetSamplers(Buffers->GetRegisterSlot(), 1, m_ResourceManager->GetSamplerState(Buffers->GetBufferName())->GetSamplerState().GetAddressOf());
		}
	}

	//PS데이터 밀어넣기
	{
		//ConstantBuffer
		auto psConstantBuffers = shaderSet.m_PixelShader->GetConstantBuffer();
		for (const auto& Buffers : psConstantBuffers)
		{
			ComPtr<ID3D11Buffer> ConstantBuffer = Buffers->GetConstantBuffer();

			context->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
			memcpy(mappedSubresource.pData, bufferSrc, Buffers->GetBufferSize());
			context->Unmap(ConstantBuffer.Get(), 0);

			context->PSSetConstantBuffers(Buffers->GetRegisterSlot(), 1, Buffers->GetConstantBuffer().GetAddressOf());
		}

		//Sampler
		auto psSamplerBuff = shaderSet.m_PixelShader->GetSamplerBuffer();
		for (const auto& Buffers : psSamplerBuff)
		{
			context->PSSetSamplers(Buffers->GetRegisterSlot(), 1, m_ResourceManager->GetSamplerState(Buffers->GetBufferName())->GetSamplerState().GetAddressOf());
		}
	}
}
void DXGraphicsEngine::SetTextures(GRAPHICSENGINE_SHADER_RESOURCE_INPUT_LAYOUT* resourceInputLayout, UINT resourceInputLayoutCnt)
{
	std::vector<ComPtr<ID3D11ShaderResourceView>> bindingTextures;
	//m_Device->Flush();

	for (UINT inputlayoutIdx = 0; inputlayoutIdx < resourceInputLayoutCnt; inputlayoutIdx++)
	{
		auto inputlayout = resourceInputLayout[inputlayoutIdx];

		switch (inputlayout.m_ResourceSource)
		{
		case SHADER_RESOURCE_SOURCE::BACKBUFFER:
			{
				//bindingTextures.emplace_back(m_BasicPass->GetShaderResourceView());
			}
			break;
		case SHADER_RESOURCE_SOURCE::PASS:
			{
				bindingTextures.push_back(m_ResourceManager->GetPass(inputlayout.m_ResourceName)->GetShaderResourceView((UINT)inputlayout.m_ResourceIndex));
			}
			break;
		case SHADER_RESOURCE_SOURCE::PASSDEPTH:
			{
				bindingTextures.push_back(m_ResourceManager->GetPass(inputlayout.m_ResourceName)->GetDepthStencilView()->GetShaderResourceView());
			}
			break;
		case SHADER_RESOURCE_SOURCE::TEXTURE:
			{
				bindingTextures.push_back(m_ResourceManager->GetTexture(inputlayout.m_ResourceIndex)->GetTextureSRV());
			}
			break;
		default:
			break;
		}
	}

	//SRV
	auto psSRV = m_BindingPostProcessPass->GetShaderSet(L"").m_PixelShader->GetTextureResourceBuffer();
	int textureidx = 0;
	for (const auto& Buffers : psSRV)
	{
		m_Device->GetContext()->VSSetShaderResources(Buffers->GetRegisterSlot(), 1, bindingTextures[textureidx].GetAddressOf());
		m_Device->GetContext()->PSSetShaderResources(Buffers->GetRegisterSlot(), 1, bindingTextures[textureidx].GetAddressOf());
		textureidx++;
	}

}
void DXGraphicsEngine::ExecutePass()
{
	auto context = m_Device->GetContext();

	m_BindingPostProcessPass->End(m_Device);
}

void DXGraphicsEngine::StartRender_Excute()
{
	// 그려질 화면 크기를 설정 및 초기화
	m_Device->Flush();
	ComPtr<ID3D11DeviceContext> Context = m_Device->GetContext();
	Context->ClearRenderTargetView(m_SwapChain->GetBackBufferRenderTarget().Get(), DirectX::Colors::Transparent);

	m_BindingPostProcessPass.reset();
	m_BindingRenderPass.reset();

	////basic pass의 렌더타겟도 초기화
	//m_BasicPass->Begin(m_Device, m_BasicPass->GetDepthStencilView());
	//Context->ClearDepthStencilView(m_DepthStencilView->GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	////context->OMSetRenderTargets(1, m_SwapChain->GetBackBufferRenderTarget().GetAddressOf(), m_DepthStencilView->GetDepthStencilView().Get());
	//Context->OMSetDepthStencilState(m_BasicPass->GetDepthStencilView()->GetDepthStencilState().Get(), 0);

}

void DXGraphicsEngine::Render_Execute(size_t MeshID, void* bufferSrc)
{
	auto context = m_Device->GetContext();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	MeshInfo* nowMesh = m_RenderInfoManager->GetMeshInfo(MeshID);
	auto shaderSet = m_BindingRenderPass->GetShaderSet(nowMesh->m_PassName);

	auto vs = shaderSet.m_VertexShader;
	auto ps = shaderSet.m_PixelShader;

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	//VS데이터 밀어넣기
	{
		//ConstantBuffer
		auto vsConstantBuffers = vs->GetConstantBuffer();
		for (const auto& Buffers : vsConstantBuffers)
		{
			ComPtr<ID3D11Buffer> ConstantBuffer = Buffers->GetConstantBuffer();

			context->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
			memcpy(mappedSubresource.pData, bufferSrc, Buffers->GetBufferSize());
			context->Unmap(ConstantBuffer.Get(), 0);

			context->VSSetConstantBuffers(Buffers->GetRegisterSlot(), 1, Buffers->GetConstantBuffer().GetAddressOf());
		}

		//SRV
		auto vsSRVBuff = vs->GetTextureResourceBuffer();
		int textureidx = 0;
		for (const auto& Buffers : vsSRVBuff)
		{
			context->VSSetShaderResources(Buffers->GetRegisterSlot(), 1, m_ResourceManager->GetTexture(nowMesh->m_TextureID[textureidx])->GetTextureSRV().GetAddressOf());
			textureidx++;
		}
		//Sampler
		auto vsSamplerBuff = vs->GetSamplerBuffer();
		for (const auto& Buffers : vsSamplerBuff)
		{
			context->VSSetSamplers(Buffers->GetRegisterSlot(), 1, m_ResourceManager->GetSamplerState(Buffers->GetBufferName())->GetSamplerState().GetAddressOf());
		}
	}

	//PS데이터 밀어넣기
	{
		//ConstantBuffer
		auto psConstantBuffers = ps->GetConstantBuffer();
		for (const auto& Buffers : psConstantBuffers)
		{
			ComPtr<ID3D11Buffer> ConstantBuffer = Buffers->GetConstantBuffer();

			context->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
			memcpy(mappedSubresource.pData, bufferSrc, Buffers->GetBufferSize());
			context->Unmap(ConstantBuffer.Get(), 0);

			context->PSSetConstantBuffers(Buffers->GetRegisterSlot(), 1, Buffers->GetConstantBuffer().GetAddressOf());
		}
		//SRV
		auto psSRVBuff = ps->GetTextureResourceBuffer();
		int textureidx = 0;
		if (nowMesh->m_TextureCnt > 0)
		{
			for (const auto& Buffers : psSRVBuff)
			{
				context->PSSetShaderResources(Buffers->GetRegisterSlot(), 1, m_ResourceManager->GetTexture(nowMesh->m_TextureID[textureidx])->GetTextureSRV().GetAddressOf());
				textureidx++;
			}
		}

		//Sampler
		auto psSamplerBuff = ps->GetSamplerBuffer();
		for (const auto& Buffers : psSamplerBuff)
		{
			context->PSSetSamplers(Buffers->GetRegisterSlot(), 1, m_ResourceManager->GetSamplerState(Buffers->GetBufferName())->GetSamplerState().GetAddressOf());
		}
	}

	////////////////////////////////////////
	auto vertices = m_ResourceManager->GetVertexBuffer(nowMesh->m_MeshId, nowMesh->m_subMeshId);
	if (vertices == nullptr)
		return;
	auto indices = m_ResourceManager->GetIndexBuffer(nowMesh->m_MeshId, nowMesh->m_subMeshId);
	if (indices == nullptr)
		return;


	//버텍스버퍼, 인덱스 버퍼 밀어넣기
	uint32 stride = vertices->GetVertexStructSize();
	uint32 offset = 0;

	context->IASetVertexBuffers(0, 1, vertices->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(indices->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	//쉐이더 설정
	context->VSSetShader(vs->GetVertexShader().Get(), nullptr, 0);
	context->PSSetShader(ps->GetPixelShader().Get(), nullptr, 0);
	context->IASetInputLayout(vs->GetInputLayout().Get());

	//그리기
	context->DrawIndexed(indices->GetIndexCount(), 0, 0);
}

void DXGraphicsEngine::Present_Execute()
{
	auto context = m_Device->GetContext();

	//MRT DBG
	const DirectX::XMFLOAT2 BoxSize{ 260, 170 };
	m_SpriteBatch->Begin();

	auto BasicPass = m_ResourceManager->GetPass(L"Basic");

	for (int i = 0; i < 4; i++)
	{
		RECT pos{ i * (int)BoxSize.x, m_ScreenInfo.m_ScreenHeight - (int)BoxSize.y,  (i + 1) * (int)BoxSize.x, m_ScreenInfo.m_ScreenHeight };
		m_SpriteBatch->Draw(BasicPass->GetShaderResourceView(i).Get(), pos);
	};

	RECT pos{ 4 * (int)BoxSize.x, m_ScreenInfo.m_ScreenHeight - (int)BoxSize.y,  (5) * (int)BoxSize.x, m_ScreenInfo.m_ScreenHeight };
	m_SpriteBatch->Draw(m_ResourceManager->GetPass(L"Shadow")->GetDepthStencilView()->GetShaderResourceView().Get(), pos);

	m_SpriteBatch->End();
	m_SwapChain->Present();

}

void DXGraphicsEngine::SetViewport(float width, float height)
{
	D3D11_VIEWPORT ViewPort;

	ViewPort.TopLeftX = 0.0f;
	ViewPort.TopLeftY = 0.0f;
	if (width <= 0)
	{
		ViewPort.Width = m_ScreenInfo.m_ScreenWidth;
	}
	else
	{
		ViewPort.Width = width;

	}

	if (height <= 0)
	{
		ViewPort.Height = m_ScreenInfo.m_ScreenHeight;
	}
	else
	{
		ViewPort.Height = height;
	}

	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;

	m_Device->GetContext()->RSSetViewports(1, &ViewPort);
}

void DXGraphicsEngine::Finalize()
{
	m_SwapChain->Finalize();
	//TODO 클래스와 시킬것.

	m_SwapChain.reset();

	m_ResourceManager->Finalize();

	m_Device->Finalize();
	m_Device.reset();
}

bool DXGraphicsEngine::CheckResourceData(size_t MeshID)
{
	return false;
}

void DXGraphicsEngine::CreateShaderPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc)
{
	m_ResourceManager->CreateShaderPass(m_ScreenInfo, shaderPassDesc);
}

void DXGraphicsEngine::CreateRenderPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc)
{
	m_ResourceManager->CreateRenderPass(m_ScreenInfo, shaderPassDesc);
}

void DXGraphicsEngine::CreatePostProcessPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc)
{
	m_ResourceManager->CreateShaderPass(m_ScreenInfo, shaderPassDesc);
}

void DXGraphicsEngine::CreateVertexBuffer(void* vertices, int vertexStructSize, int vertexCount, size_t MeshID, UINT subMeshID)
{
	m_ResourceManager->CreateVertexBuffer(vertices, vertexStructSize, vertexCount, MeshID, subMeshID);
}
void DXGraphicsEngine::CreateIndexBuffer(void* indices, int indexCount, size_t MeshID, UINT subMeshID)
{
	m_ResourceManager->CreateIndexBuffer(indices, indexCount, MeshID, subMeshID);
}
void DXGraphicsEngine::CreateTexture(std::wstring path, size_t TextureID)
{
	m_ResourceManager->CreateTexture(path, TextureID);
}
void DXGraphicsEngine::CreateSamplerState(size_t SamplerID)
{

}

void DXGraphicsEngine::CreateRasterizerState(std::wstring RasterizerID, const GRAPHICSENGINE_RASTERIZER_DESC& rasterizerDesc)
{
	m_ResourceManager->CreateRasterizerState(RasterizerID, rasterizerDesc);
}

void DXGraphicsEngine::DeleteMesh(size_t MeshID)
{
	m_ResourceManager->DeleteMesh(MeshID);
}

void DXGraphicsEngine::DeleteVertexBuffer(size_t MeshID, UINT subMeshID)
{
	m_ResourceManager->DeleteVertexBuffer(MeshID, subMeshID);
}

void DXGraphicsEngine::DeleteIndexBuffer(size_t MeshID, UINT subMeshID)
{
	m_ResourceManager->DeleteIndexBuffer(MeshID, subMeshID);
}

void DXGraphicsEngine::DeleteTexture(size_t TextureID)
{
	m_ResourceManager->DeleteTexture(TextureID);
}

void DXGraphicsEngine::DeleteSamplerState(size_t SamplerID)
{
}


void DXGraphicsEngine::CreateRenderState()
{
	//// Render_Execute State 중 Rasterizer State
	//D3D11_RASTERIZER_DESC solidDesc;
	//ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	//solidDesc.FillMode = D3D11_FILL_SOLID;
	//solidDesc.CullMode = D3D11_CULL_NONE;

	////카메라에서 봤을떄... 
	//solidDesc.FrontCounterClockwise = false;

	//solidDesc.AntialiasedLineEnable = true;
	//solidDesc.MultisampleEnable = true;
	//solidDesc.ScissorEnable = false;

	//solidDesc.DepthBias = 0;
	//solidDesc.DepthBiasClamp = 0.0f;
	//solidDesc.DepthClipEnable = true;
	//solidDesc.SlopeScaledDepthBias = 0.0f;


	//m_Device->GetDevice()->CreateRasterizerState(&solidDesc, &m_SolidRS);

	//D3D11_RASTERIZER_DESC wireDesc;
	//ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	//wireDesc.FillMode = D3D11_FILL_WIREFRAME;
	//wireDesc.CullMode = D3D11_CULL_NONE;

	////카메라에서 봤을떄... 
	//wireDesc.FrontCounterClockwise = false;

	//wireDesc.AntialiasedLineEnable = true;
	//wireDesc.MultisampleEnable = true;
	//wireDesc.ScissorEnable = false;

	//wireDesc.DepthBias = 0;
	//wireDesc.DepthBiasClamp = 0.0f;
	//wireDesc.DepthClipEnable = true;
	//wireDesc.SlopeScaledDepthBias = 0.0f;

	//m_Device->GetDevice()->CreateRasterizerState(&wireDesc, &m_WireRS);
}
