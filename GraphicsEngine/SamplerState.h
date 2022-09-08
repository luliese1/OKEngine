#pragma once

//샘플러 버퍼
class Device;

class SamplerState
{
enum class SamplerType
{
	Linear,
	Anisotropic
};

public:
	void Initialize(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE textureOutside, std::shared_ptr<Device> device);
	void Release();

	Microsoft::WRL::ComPtr<ID3D11SamplerState> GetSamplerState() const { return m_SamplerState; }
private:
	ComPtr<ID3D11SamplerState> m_SamplerState;
	UINT m_SlotIdx;

};

