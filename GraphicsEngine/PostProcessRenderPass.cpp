#include "pch.h"
#include "PostProcessRenderPass.h"
#include "Device.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h" 

#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "DepthStencilView.h"
#include "RenderTargetView.h"


PostProcessRenderPass::PostProcessRenderPass() : PassBase(ePassType::PostProcess)
{
}

void PostProcessRenderPass::Initialize(ScreenInfo& sinfo, const GRAPHICSENGINE_PASS_DESC& passDesc, std::shared_ptr<Device> device)
{
	m_PassName = passDesc.m_PassName;
	m_RasterizerState = passDesc.m_RasterizerStateName;

	if (m_VertexShader == nullptr)
	{
		m_VertexShader = std::make_shared<VertexShader>();
		m_VertexShader->Initialize(L"./Shader/PostProcess.hlsl", nullptr, 0, device);

		m_ViewBox = std::make_shared<ViewBox>();
		m_ViewBox->Initialize(device);
	}

	//쉐이더 셋 지정
	for (int shaderSetCnt = 0; shaderSetCnt < 1; shaderSetCnt++)
	{
		auto shaderDesc = passDesc.m_ShaderDesc[shaderSetCnt];

		std::shared_ptr<PixelShader> pixelShader = std::make_shared<PixelShader>();
		pixelShader->Initialize(shaderDesc.m_PixelShaderPath, shaderDesc.m_ShaderMacro, shaderDesc.m_ShaderMacroCnt, device);

		m_PixelShader = pixelShader;
	}

	m_RenderTargetTextureCnt = passDesc.m_OutputTexturesCount;

	if (m_RenderTargetTextureCnt > 8 || m_RenderTargetTextureCnt < 1)
	{
		assert("RenderPass::Initialize | RTT Texture Count Error");
	}

	for (int cnt = 0; cnt < m_RenderTargetTextureCnt; cnt++)
	{
		switch (passDesc.m_RenderTargetLayout[cnt].m_ResourceSource)
		{
		case SHADER_RENDER_TARGET::BACKBUFFER:
			{
				m_RenderTargetInfo.emplace_back(passDesc.m_RenderTargetLayout[cnt]);
			}
			break;
		case SHADER_RENDER_TARGET::OTHERPASS:
			{
				m_RenderTargetInfo.emplace_back(passDesc.m_RenderTargetLayout[cnt]);
			}
			break;
		case SHADER_RENDER_TARGET::PASS:
			{
				std::shared_ptr<RenderTargetView> tempRTT = std::make_shared<RenderTargetView>();
				tempRTT->Initialize(DXGI_FORMAT_R8G8B8A8_UNORM, sinfo, device);
				m_RenderTargetTexture.emplace_back(tempRTT);

				m_RenderTargetInfo.emplace_back(passDesc.m_RenderTargetLayout[cnt]);
			}
			break;
		default:
			Assert("PostProcessingRenderPass : Initialize ShaderResourceSource Error ");
			break;
		}
	}
}

void PostProcessRenderPass::Begin(std::shared_ptr<Device> device, std::shared_ptr<DepthStencilView> dsv)
{
	auto context = device->GetContext();

	for (auto iter : m_BindingRenderTargetTexture)
	{
		context->ClearRenderTargetView(iter.Get(), DirectX::Colors::White);
	}
	//SRV 바인딩은 바깥에서.
	//context->OMSetRenderTargets(m_RenderTargetTextureCnt, m_BindingRenderTargetTexture[0].GetAddressOf(), dsv->GetDepthStencilView().Get());

	uint32 stride = m_ViewBox->GetVertexBuffer()->GetVertexStructSize();
	uint32 offset = 0;

	context->IASetVertexBuffers(0, 1, m_ViewBox->GetVertexBuffer()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_ViewBox->GetIndexBuffer()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	//쉐이더 설정
	context->VSSetShader(m_VertexShader->GetVertexShader().Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader->GetPixelShader().Get(), nullptr, 0);

	context->IASetInputLayout(m_VertexShader->GetInputLayout().Get());
}

ShaderSet PostProcessRenderPass::GetShaderSet(const std::wstring& shaderName)
{
	ShaderSet temp;
	temp.m_VertexShader = m_VertexShader;
	//auto find = m_PixelShaders.find(shaderName);
	//if (m_PixelShaders.end() == find)
	//	temp.m_PixelShader = nullptr;
	//else
	//	temp.m_PixelShader = find->second;
	temp.m_PixelShader = m_PixelShader;


	return temp;
}

void PostProcessRenderPass::End(std::shared_ptr<Device> device)
{
	device->GetContext()->DrawIndexed(m_ViewBox->GetIndexBuffer()->GetIndexCount(), 0, 0);
}

void PostProcessRenderPass::Finalize()
{

}

void PostProcessRenderPass::UpdatePerFrameBuffer()
{

}

void PostProcessRenderPass::UpdatePerObjectBuffer()
{

}

std::shared_ptr<ViewBox> PostProcessRenderPass::m_ViewBox;
std::shared_ptr<VertexShader> PostProcessRenderPass::m_VertexShader;
