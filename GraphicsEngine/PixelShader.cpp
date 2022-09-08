#include "pch.h"
#include "PixelShader.h"
#include "Device.h"
#include "RenderTargetView.h"

//https://docs.microsoft.com/ko-kr/windows/win32/direct3d11/how-to--compile-a-shader

bool PixelShader::Initialize(std::wstring path, GRAPHICSENGINE_SHADER_MACRO_DESC* macro, UINT macroCnt, std::shared_ptr<Device>& device)
{
	//blob에 데이터를 저장?로드함?
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif

	ComPtr<ID3DBlob> errorBlob = nullptr;
	ComPtr<ID3DBlob>& ShaderBlob = GetBlob();

	std::vector<D3D_SHADER_MACRO> macroVector = CreateD3DShaderMacro(macro, macroCnt);
	D3D_SHADER_MACRO* bindMacro = NULL;
	if (macroVector.size() > 1)
	{
		bindMacro = &macroVector.front();
	}

	HRESULT hr = D3DCompileFromFile(path.c_str(), bindMacro, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PSMain", "ps_5_0", //entrypoint , profile
		flags, 0, //플래그들,
		ShaderBlob.GetAddressOf(),
		errorBlob.GetAddressOf());

	if (FAILED(hr) == true)
	{
		if (errorBlob.Get())
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (ShaderBlob)
			ShaderBlob->Release();

		return FAILED(hr) ? false : true;
	}

	hr = device->GetDevice()->CreatePixelShader(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), nullptr, m_PixelShader.GetAddressOf());
	
	ComPtr<ID3D11ShaderReflection> ShaderReflector = nullptr;
	D3DReflect(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)ShaderReflector.GetAddressOf());

	//CreateOutputLayout(ShaderReflector);
	CreateShaderBindBuffers(ShaderReflector, device);
	return FAILED(hr)? false : true;
}

void PixelShader::Release()
{
	m_PixelShader.ReleaseAndGetAddressOf();
	GetBlob().ReleaseAndGetAddressOf();
}


//void PixelShader::CreateOutputLayout(ComPtr<ID3D11ShaderReflection> ShaderReflector)
//{
//	D3D11_SHADER_DESC ShaderDesc;
//	ShaderReflector->GetDesc(&ShaderDesc);
//
//	ID3D11ShaderReflectionConstantBuffer* bufferInfo;
//
//	D3D11_SHADER_BUFFER_DESC bufferDesc;
//
//	std::vector<D3D11_SIGNATURE_PARAMETER_DESC> Data;
//
//
//	for (unsigned outputIndex = 0; outputIndex < ShaderDesc.OutputParameters; outputIndex++)
//	{
//		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
//		ShaderReflector->GetOutputParameterDesc(outputIndex, &paramDesc);
//
//		Data.emplace_back(paramDesc);
//	}
//}

