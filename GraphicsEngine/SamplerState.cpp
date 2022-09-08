#include "pch.h"
#include "SamplerState.h"
#include "Device.h"

void SamplerState::Initialize(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE textureOutside, std::shared_ptr<Device> device)
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = filter;
	samplerDesc.AddressU = textureOutside;
	samplerDesc.AddressW = textureOutside;
	samplerDesc.AddressV = textureOutside;

	device->GetDevice()->CreateSamplerState(&samplerDesc, m_SamplerState.GetAddressOf());
}

void SamplerState::Release()
{
	m_SamplerState.Reset();
}