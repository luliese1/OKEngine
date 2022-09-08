#include "pch.h"
#include "RenderPass.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderTargetView.h"
#include "Device.h"
#include "DepthStencilView.h"

RenderPass::RenderPass() : PassBase(ePassType::RenderPass), m_DepthStencilView(std::make_shared<DepthStencilView>())
{
}

void RenderPass::Initialize(ScreenInfo& sinfo, const GRAPHICSENGINE_PASS_DESC& passDesc, std::shared_ptr<Device> device)
{
	m_PassName = passDesc.m_PassName;

	m_TextureSizeInfo.m_ScreenHeight = passDesc.m_TextureHeight;
	m_TextureSizeInfo.m_ScreenWidth = passDesc.m_TextureWidth;
	m_TextureSizeInfo.m_4xMsaaFlag = sinfo.m_4xMsaaFlag;
	m_TextureSizeInfo.m_4xMsaaQuality = sinfo.m_4xMsaaQuality;

	ScreenInfo tempInfo = m_TextureSizeInfo;
	if (m_TextureSizeInfo.m_ScreenHeight <= 0)
	{
		tempInfo.m_ScreenHeight = sinfo.m_ScreenHeight;
	}
	if (m_TextureSizeInfo.m_ScreenWidth <= 0)
	{
		tempInfo.m_ScreenWidth = sinfo.m_ScreenWidth;
	}


	m_DepthStencilView->Initialize(device, tempInfo);

	//���̴� �� ����
	for (int shaderSetCnt = 0; shaderSetCnt < passDesc.m_ShaderCount; shaderSetCnt++)
	{
		ShaderSet tempShaderSet;

		auto shaderDesc = passDesc.m_ShaderDesc[shaderSetCnt];

		tempShaderSet.m_VertexShader = std::make_shared<VertexShader>();
		tempShaderSet.m_VertexShader->Initialize(shaderDesc.m_VertexShaderPath, shaderDesc.m_ShaderMacro, shaderDesc.m_ShaderMacroCnt, device);
		tempShaderSet.m_PixelShader = std::make_shared<PixelShader>();
		tempShaderSet.m_PixelShader->Initialize(shaderDesc.m_PixelShaderPath, shaderDesc.m_ShaderMacro, shaderDesc.m_ShaderMacroCnt, device);

		m_ShaderSet.emplace(shaderDesc.m_ShaderSetName, tempShaderSet);
	}

	m_RenderTargetTextureCnt = passDesc.m_OutputTexturesCount;

	if (m_RenderTargetTextureCnt > 8 || m_RenderTargetTextureCnt < 1)
	{
		Assert("RenderPass::Initialize | RTT Texture Count Error");
	}

	for (int cnt = 0; cnt < m_RenderTargetTextureCnt; cnt++)
	{
		std::shared_ptr<RenderTargetView> tempRTT = std::make_shared<RenderTargetView>();
		tempRTT->Initialize(DXGI_FORMAT_R8G8B8A8_UNORM, tempInfo, device);
		m_RenderTargetTexture.emplace_back(tempRTT);
	}

	SetRenderTargetViewListForBinding();
}

void RenderPass::Begin(std::shared_ptr<Device> device, std::shared_ptr<DepthStencilView> dsv)
{
	auto context = device->GetContext();

	for (auto iter : m_BindingRenderTargetTexture)
	{
		context->ClearRenderTargetView(iter.Get(), DirectX::Colors::White);
	}

	context->OMSetRenderTargets(m_RenderTargetTextureCnt, m_BindingRenderTargetTexture[0].GetAddressOf(), dsv->GetDepthStencilView().Get());
}

ShaderSet RenderPass::GetShaderSet(const std::wstring& shaderName)
{
	auto shaderSet = m_ShaderSet.find(shaderName);
	if (shaderSet == m_ShaderSet.end())
		return ShaderSet();

	return shaderSet->second;
}

void RenderPass::OnResize(ScreenInfo& sinfo, std::shared_ptr<Device> device)
{
	if (m_TextureSizeInfo.m_ScreenHeight <= 0 || m_TextureSizeInfo.m_ScreenWidth <= 0)
	{
		ScreenInfo tempInfo = m_TextureSizeInfo;
		if (m_TextureSizeInfo.m_ScreenHeight <= 0)
		{
			tempInfo.m_ScreenHeight = sinfo.m_ScreenHeight;
		}
		if (m_TextureSizeInfo.m_ScreenWidth <= 0)
		{
			tempInfo.m_ScreenWidth = sinfo.m_ScreenWidth;
		}

		for (auto iter : m_RenderTargetTexture)
		{
			iter->OnResize(DXGI_FORMAT_R8G8B8A8_UNORM, tempInfo, device);
		}

		m_DepthStencilView->OnResize(tempInfo, DXGI_FORMAT_D24_UNORM_S8_UINT, device);

		SetRenderTargetViewListForBinding();
	}
}

void RenderPass::End(std::shared_ptr<Device> device)
{
	if (m_TextureSizeInfo.m_4xMsaaFlag)
	{
		for (auto iter : m_RenderTargetTexture)
		{
			iter->ResolveSubresource(device);
		}
	}
}

void RenderPass::Finalize()
{
	ShaderBase::ReleasePerFrameBuffer();

	m_ShaderSet.clear();
	m_RenderTargetTexture.clear();
}

void RenderPass::UpdatePerFrameBuffer()
{

}

void RenderPass::UpdatePerObjectBuffer()
{

}
