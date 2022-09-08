#pragma once
#include "ShaderBase.h"

class Device;

//버텍스쉐이더

class VertexShader : public ShaderBase
{
public:
	bool Initialize(std::wstring path, GRAPHICSENGINE_SHADER_MACRO_DESC* macro, UINT macroCnt, std::shared_ptr<Device>& device);
	void Release();

	ComPtr<ID3D11VertexShader> GetVertexShader(){ return m_VertexShader; }
	void SetInputLayout(ComPtr<ID3D11InputLayout> input) {m_InputLayout = input;}
	ComPtr<ID3D11InputLayout> GetInputLayout() { return m_InputLayout; }

	//ID3D11InputLayout** GetInputLayoutPtr() { return m_InputLayout.GetAddressOf();}
private:
	void CreateInputLayout(ComPtr<ID3D11ShaderReflection> ShaderReflector, std::shared_ptr<Device>& device);

	// 쉐이더 코드덩어리를 물고있는 놈.
	ComPtr<ID3D11VertexShader> m_VertexShader;
	ComPtr<ID3D11InputLayout> m_InputLayout;
};

