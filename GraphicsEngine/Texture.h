#pragma once

class Device;

class Texture
{
public:
	bool Initialize(std::wstring path, std::shared_ptr<Device> device);
	void Release();

	ComPtr<ID3D11ShaderResourceView> GetTextureSRV() const { return m_TextureSRV; }

private:
	ComPtr<ID3D11Resource> GetTextureResource() const { return m_TextureResource; }
	ComPtr<ID3D11Resource> m_TextureResource;

	ComPtr<ID3D11ShaderResourceView> m_TextureSRV; 
};

