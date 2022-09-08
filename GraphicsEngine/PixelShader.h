#pragma once
#include "ShaderBase.h"

class Device;
class RenderTargetView;
//�ȼ� ���̴� ������

class PixelShader : public ShaderBase
{
public:
	bool Initialize(std::wstring path, GRAPHICSENGINE_SHADER_MACRO_DESC* macro, UINT macroCnt, std::shared_ptr<Device>& device) override;
	void Release();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader() const { return m_PixelShader; }

private:

	// ���̴� �ڵ嵢��� �����ִ� ��.
	ComPtr<ID3D11PixelShader> m_PixelShader;
};

