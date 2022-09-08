#pragma once
#include "ShaderBase.h"

class Device;
class RenderTargetView;
//픽셀 쉐이더 보관함

class PixelShader : public ShaderBase
{
public:
	bool Initialize(std::wstring path, GRAPHICSENGINE_SHADER_MACRO_DESC* macro, UINT macroCnt, std::shared_ptr<Device>& device) override;
	void Release();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader() const { return m_PixelShader; }

private:

	// 쉐이더 코드덩어리를 물고있는 놈.
	ComPtr<ID3D11PixelShader> m_PixelShader;
};

