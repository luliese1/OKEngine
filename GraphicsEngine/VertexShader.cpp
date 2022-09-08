#include "pch.h"
#include "VertexShader.h"
#include "Device.h"

bool VertexShader::Initialize(std::wstring path, GRAPHICSENGINE_SHADER_MACRO_DESC* macro, UINT macroCnt, std::shared_ptr<Device>& device)
{
	//blob에 데이터를 저장?로드함?
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif

	ComPtr<ID3DBlob> errorBlob = nullptr;

	std::vector<D3D_SHADER_MACRO> macroVector = CreateD3DShaderMacro(macro, macroCnt);
	D3D_SHADER_MACRO* bindMacro = NULL;
	if (macroVector.size() > 1)
	{
		bindMacro = &macroVector.front();
	}

	HRESULT hrresult = S_FALSE;

	hrresult = D3DCompileFromFile(path.c_str(), bindMacro, D3D_COMPILE_STANDARD_FILE_INCLUDE ,
		"VSMain", "vs_5_0", //entrypoint , profile
		flags, 0, //플래그들,
		m_ShaderBlob.GetAddressOf(),
		errorBlob.GetAddressOf());

	HR(hrresult);
	
	if (FAILED(hrresult))
	{
		if (errorBlob.Get())
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (m_ShaderBlob)
			m_ShaderBlob->Release();

		return FAILED(hrresult) ? false : true;
	}

	hrresult = device->GetDevice()->CreateVertexShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, m_VertexShader.GetAddressOf());

	ComPtr<ID3D11ShaderReflection> ShaderReflector = nullptr;

	D3DReflect(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)ShaderReflector.GetAddressOf());
	CreateInputLayout(ShaderReflector, device);
	CreateShaderBindBuffers(ShaderReflector, device);

	return FAILED(hrresult) ? false : true;
}

void VertexShader::Release()
{
	m_VertexShader.ReleaseAndGetAddressOf();
	GetBlob().ReleaseAndGetAddressOf();
}

void VertexShader::CreateInputLayout(ComPtr<ID3D11ShaderReflection> ShaderReflector, std::shared_ptr<Device>& device)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc;

	D3D11_SHADER_DESC ShaderDesc;
	ShaderReflector->GetDesc(&ShaderDesc);

	for (unsigned InputIndex = 0; InputIndex < ShaderDesc.InputParameters ; InputIndex++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		ShaderReflector->GetInputParameterDesc(InputIndex, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC elementDesc;

		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex; //동일한 시멘틱스를 사용하는 매개변수 구분
		elementDesc.InputSlot = 0; //d3d11에 정의된 0~15값 input assembler
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; //편의를 위해 이걸로, 
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		//Mask which indicates which components of a register are used.

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		InputLayoutDesc.push_back(elementDesc);
	}

	HR(device->GetDevice()->CreateInputLayout(&InputLayoutDesc.front(), InputLayoutDesc.size(), m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), m_InputLayout.GetAddressOf()));
}
