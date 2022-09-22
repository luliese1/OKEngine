#include "pch.h"
#include "RenderPassBase.h"
#include "RenderTargetView.h"

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> PassBase::GetShaderResourceView(UINT idx)
{
	if (idx >= m_RenderTargetTextureCnt)
	{
		assert("RednerPassBase::GetShaderResourceView | 해당 텍스쳐가 존재하지 않습니다");
		return nullptr;
	}

	return m_RenderTargetTexture[idx]->GetShaderResourceView();
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> PassBase::GetRenderTargetView(UINT idx)
{
	if (idx >= m_RenderTargetTextureCnt)
	{
		assert("RednerPassBase::GetRenderTargetView | 해당 텍스쳐가 존재하지 않습니다");
		return nullptr;
	}

	return m_RenderTargetTexture[idx]->GetRenderTargetView();
}

PassBase::PassBase(ePassType passType) : m_PassType(passType)
{

}

void PassBase::OnResize(ScreenInfo& sinfo, std::shared_ptr<Device> device)
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
			iter->OnResize(DXGI_FORMAT_R32G32B32A32_FLOAT, tempInfo, device);
		}

		SetRenderTargetViewListForBinding();
	}
}

void PassBase::SetRenderTargetViewListForBinding()
{
	m_BindingRenderTargetTexture.clear();
	m_BindingRenderTargetTexture.reserve(m_RenderTargetTexture.size());

	for (auto iter : m_RenderTargetTexture)
	{
		m_BindingRenderTargetTexture.emplace_back(iter->GetRenderTargetView());
	}
}
