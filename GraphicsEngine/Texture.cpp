#include "pch.h"
#include "Texture.h"
#include "Device.h"
#include "DDSTextureLoader.h"

bool Texture::Initialize(std::wstring path, std::shared_ptr<Device> device)
{
	HRESULT hr = S_FALSE;
	hr = DirectX::CreateDDSTextureFromFile(device->GetDevice().Get(), path.c_str(), m_TextureResource.GetAddressOf(), m_TextureSRV.GetAddressOf());
	//HR(hr);

	return FAILED(hr) ? false : true;
}

void Texture::Release()
{
	m_TextureSRV.ReleaseAndGetAddressOf();
}
