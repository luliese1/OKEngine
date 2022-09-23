#include "pch.h"
#include "DXResourceManager.h"
#include "RenderStruct.h"
#include "Device.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "SamplerState.h"

#include "RenderPass.h"
#include "PostProcessRenderPass.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "TextureResourceBuffer.h"
#include "SamplerBuffer.h"

void DXResourceManager::Initialize(std::shared_ptr<Device>& device)
{
	m_Device = device;
	HR(device->GetDevice()->CreateClassLinkage(m_ShaderDynamicLinkage.GetAddressOf()));

	//테스트함수
	TestFunction();
}

void DXResourceManager::Finalize()
{
	m_Device.reset();

	m_ShaderDynamicLinkage.Reset();

	m_Meshs.clear();

	m_Textures.clear();
	m_Materials.clear();

	m_PixelShaders.clear();
	m_VertexShaders.clear();

	m_SamplerState.clear();
}

void DXResourceManager::CreateIndexBuffer(void* indices, int indexCount, size_t ObjecID, UINT index)
{
	if (m_Device == nullptr)
		Assert("device null error : CreateIndexBuffer");

	//기존의 메쉬번호와 일치하지 않는다면..
	auto foundMesh = m_Meshs.find(ObjecID);
	std::shared_ptr<Mesh> mesh;

	if (foundMesh == m_Meshs.end())
	{
		mesh = std::make_shared<Mesh>();
		m_Meshs.emplace(ObjecID, mesh);
	}
	else
	{
		mesh = foundMesh->second;
	}

	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->Initialize(indices, indexCount, m_Device);

	mesh->m_IndexBuffers.emplace(index, indexBuffer);
}

void DXResourceManager::CreateVertexBuffer(void* vertices, int vertexStructSize, int vertexCount, size_t ObjecID, UINT index)
{
	if (m_Device == nullptr)
		Assert("device null error : CreateVertexBuffer");

	//기존의 메쉬번호와 일치하지 않는다면..
	auto foundMesh = m_Meshs.find(ObjecID);
	std::shared_ptr<Mesh> mesh;

	if (foundMesh == m_Meshs.end())
	{
		mesh = std::make_shared<Mesh>();
		m_Meshs.emplace(ObjecID, mesh);
	}
	else
	{
		mesh = foundMesh->second;
	}

	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->Initialize(vertices, vertexStructSize, vertexCount, m_Device);

	mesh->m_VertexBuffers.emplace(index, vertexBuffer);

}

void DXResourceManager::CreateTexture(tstring path, size_t ObjecID)
{
	if (m_Device == nullptr)
		Assert("device null error : CreateTexture");
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	texture->Initialize(path, m_Device);

	m_Textures.emplace(ObjecID, texture);
}

void DXResourceManager::CreateMaterial(size_t ObjecID)
{

}

void DXResourceManager::CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE textureOutside, std::wstring ObjecID)
{
	if (m_Device == nullptr)
		Assert("device null error : CreateSampler");
	std::shared_ptr<SamplerState> samplerState = std::make_shared<SamplerState>();

	samplerState->Initialize(filter, textureOutside, m_Device);
	m_SamplerState.emplace(ObjecID, samplerState);
}

void DXResourceManager::CreateRasterizerState(std::wstring RasterizerID, const GRAPHICSENGINE_RASTERIZER_DESC& rasterizerDesc)
{
	if (m_Device == nullptr)
		Assert("device null error : CreateRasterizer");

	ComPtr<ID3D11RasterizerState> rasterizerState;

	D3D11_RASTERIZER_DESC D3D11rasterizerDesc;

	D3D11rasterizerDesc.FillMode = (D3D11_FILL_MODE)(int)rasterizerDesc.m_FillMode;
	D3D11rasterizerDesc.CullMode = (D3D11_CULL_MODE)(int)rasterizerDesc.m_CullMode;

	D3D11rasterizerDesc.FrontCounterClockwise = false;

	D3D11rasterizerDesc.DepthBias = rasterizerDesc.m_DepthBias;
	D3D11rasterizerDesc.DepthBiasClamp = rasterizerDesc.m_DepthBiasClamp;
	D3D11rasterizerDesc.SlopeScaledDepthBias = rasterizerDesc.m_SlopeScaledDepthBias;

	D3D11rasterizerDesc.DepthClipEnable = rasterizerDesc.m_DepthClipEnable;
	D3D11rasterizerDesc.ScissorEnable = false;
	D3D11rasterizerDesc.MultisampleEnable = rasterizerDesc.m_MultiSampleEnable;
	D3D11rasterizerDesc.AntialiasedLineEnable = rasterizerDesc.m_AntialiasedLineEnable;

	m_Device->GetDevice()->CreateRasterizerState(&D3D11rasterizerDesc, rasterizerState.GetAddressOf());

	m_RasterizerState.emplace(RasterizerID, rasterizerState);
}

void DXResourceManager::CreateRenderPass(ScreenInfo& sinfo, const GRAPHICSENGINE_PASS_DESC& shaderPassDesc)
{
	if (m_Device == nullptr)
		Assert("device null error : CreatePass");

	std::shared_ptr<RenderPass> shaderPass = std::make_shared<RenderPass>();
	shaderPass->Initialize(sinfo, shaderPassDesc, m_Device);

	m_RenderPasses.emplace(shaderPassDesc.m_PassName, shaderPass);
}

void DXResourceManager::CreatePostProcessPass(ScreenInfo& sinfo, const GRAPHICSENGINE_PASS_DESC& shaderPassDesc)
{
	if (m_Device == nullptr)
		Assert("device null error : CreatePass");

	std::shared_ptr<PostProcessRenderPass> shaderPass = std::make_shared<PostProcessRenderPass>();
	shaderPass->Initialize(sinfo, shaderPassDesc, m_Device);

	m_RenderPasses.emplace(shaderPassDesc.m_PassName, shaderPass);
}

void DXResourceManager::CreateShaderPass(ScreenInfo& sinfo, const GRAPHICSENGINE_PASS_DESC& shaderPassDesc)
{
	if (m_Device == nullptr)
		Assert("device null error : CreatePass");

	std::shared_ptr<PostProcessRenderPass> shaderPass = std::make_shared<PostProcessRenderPass>();
	shaderPass->Initialize(sinfo, shaderPassDesc, m_Device);

	m_RenderPasses.emplace(shaderPassDesc.m_PassName, shaderPass);
}

void DXResourceManager::DeleteVertexShader(size_t ObjecID)
{
	auto find = m_VertexShaders.find(ObjecID);
	if (find == m_VertexShaders.end())
	{
		return;
	}
	else
	{
		find->second.reset();
		m_VertexShaders.erase(find);
	}
}

void DXResourceManager::DeletePixelShader(size_t ObjecID)
{
	auto find = m_PixelShaders.find(ObjecID);
	if (find == m_PixelShaders.end())
	{
		return;
	}
	else
	{
		find->second.reset();
		m_PixelShaders.erase(find);
	}
}

void DXResourceManager::DeleteMesh(size_t ObjecID)
{
	auto find = m_Meshs.find(ObjecID);
	if (find == m_Meshs.end())
	{
		return;
	}
	else
	{
		find->second.reset();
		m_Meshs.erase(find);
	}
}

void DXResourceManager::DeleteIndexBuffer(size_t ObjecID, UINT subMeshId)
{
	auto find = m_Meshs.find(ObjecID);
	if (find == m_Meshs.end())
	{
		return;
	}
	else
	{
		auto findIndexBuf = find->second->m_IndexBuffers.find(subMeshId);
		if (findIndexBuf == find->second->m_IndexBuffers.end())
		{
			return;
		}
		else
		{
			findIndexBuf->second.reset();
			find->second->m_IndexBuffers.erase(subMeshId);
		}
	}


}

void DXResourceManager::DeleteVertexBuffer(size_t ObjecID, UINT subMeshId)
{
	auto find = m_Meshs.find(ObjecID);
	if (find == m_Meshs.end())
	{
		return;
	}
	else
	{
		auto findVertexBuf = find->second->m_VertexBuffers.find(subMeshId);
		if (findVertexBuf == find->second->m_VertexBuffers.end())
		{
			return;
		}
		else
		{
			findVertexBuf->second.reset();
			find->second->m_VertexBuffers.erase(subMeshId);
		}
	}
}

void DXResourceManager::DeleteTexture(size_t ObjecID)
{
	auto find = m_Textures.find(ObjecID);
	if (find == m_Textures.end())
	{
		return;
	}
	else
	{
		find->second.reset();
		m_Textures.erase(find);
	}
}

void DXResourceManager::DeleteMaterial(size_t ObjecID)
{
	auto find = m_Materials.find(ObjecID);
	if (find == m_Materials.end())
	{
		return;
	}
	else
	{
		find->second.reset();
		m_Materials.erase(find);
	}
}

void DXResourceManager::DeleteSamplerState(std::wstring ObjecID)
{
	auto find = m_SamplerState.find(ObjecID);
	if (find == m_SamplerState.end())
	{
		return;
	}
	else
	{
		find->second.reset();
		m_SamplerState.erase(find);
	}
}

std::shared_ptr<VertexBuffer> DXResourceManager::GetVertexBuffer(size_t ObjecID, UINT subMeshId)
{

	//기존의 메쉬번호와 일치하지 않는다면..
	auto foundMesh = m_Meshs.find(ObjecID);
	std::shared_ptr<Mesh> mesh;

	if (foundMesh == m_Meshs.end())
	{
		return nullptr;
	}
	else
	{
		mesh = foundMesh->second;
	}

	auto ret = mesh->m_VertexBuffers.find(subMeshId);

	if (ret == mesh->m_VertexBuffers.end())
		return nullptr;

	return ret->second;
}

std::shared_ptr<IndexBuffer> DXResourceManager::GetIndexBuffer(size_t ObjecID, UINT subMeshId)
{
	//기존의 메쉬번호와 일치하지 않는다면..
	auto foundMesh = m_Meshs.find(ObjecID);
	std::shared_ptr<Mesh> mesh;

	if (foundMesh == m_Meshs.end())
	{
		return nullptr;
	}
	else
	{
		mesh = foundMesh->second;
	}

	auto ret = mesh->m_IndexBuffers.find(subMeshId);

	if (ret == mesh->m_IndexBuffers.end())
		return nullptr;

	return ret->second;
}

std::shared_ptr<Texture> DXResourceManager::GetTexture(size_t ObjecID)
{
	auto ret = m_Textures.find(ObjecID);
	if (ret == m_Textures.end())
		return nullptr;

	return ret->second;
}

std::shared_ptr<PassBase> DXResourceManager::GetPass(std::wstring passName)
{
	auto ret = m_RenderPasses.find(passName);
	if (ret == m_RenderPasses.end())
		return nullptr;

	return ret->second;
}

std::shared_ptr<Material> DXResourceManager::GetMaterial(size_t ObjecID)
{
	auto ret = m_Materials.find(ObjecID);
	if (ret == m_Materials.end())
		return nullptr;

	return ret->second;
}

std::shared_ptr<PixelShader> DXResourceManager::GetPixelShader(size_t ObjecID)
{
	auto ret = m_PixelShaders.find(ObjecID);
	if (ret == m_PixelShaders.end())
		return nullptr;

	return ret->second;
}

std::shared_ptr<VertexShader> DXResourceManager::GetVertexShader(size_t ObjecID)
{
	auto ret = m_VertexShaders.find(ObjecID);
	if (ret == m_VertexShaders.end())
		return nullptr;

	return ret->second;
}

std::shared_ptr<SamplerState> DXResourceManager::GetSamplerState(std::wstring ObjecID)
{
	auto ret = m_SamplerState.find(ObjecID);
	if (ret == m_SamplerState.end())
		return nullptr;

	return ret->second;
}

Microsoft::WRL::ComPtr<ID3D11RasterizerState> DXResourceManager::GetRaterizerState(std::wstring ObjecID)
{
	auto ret = m_RasterizerState.find(ObjecID);
	if (ret == m_RasterizerState.end())
		return nullptr;

	return ret->second;
}

void DXResourceManager::TestFunction()
{
	//CreateVertexShader(L"./Shader/VetexShader_StaticMesh.hlsl", 101);
	//CreatePixelShader(L"./Shader/PixelShader_StaticMesh.hlsl", 101);

	CreateTexture(L"./Resource/dog.dds", 101);

	Vertex vertices[] =
	{
		{ DirectX::XMFLOAT3(+0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
		{ DirectX::XMFLOAT3(+0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
		{ DirectX::XMFLOAT3(-0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
		{ DirectX::XMFLOAT3(+0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
		{ DirectX::XMFLOAT3(+0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
		{ DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
		{ DirectX::XMFLOAT3(-0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT3(0.f , 0.f , 0.f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
	};
	int verticesSize = ARRAYSIZE(vertices);
	// Index
	UINT32 indices[] =
	{
		0, 3, 1,
		2, 1, 3,
		0, 4, 3,
		7, 3, 4,
		7, 2, 3,
		2, 7, 6,
		4, 0, 5,
		1, 5, 0,
		4, 5, 7,
		6, 7, 5,
		1, 2, 5,
		6, 5, 2
	};
	int indicesSize = ARRAYSIZE(indices);

	CreateVertexBuffer(vertices, sizeof(Vertex), verticesSize, 101, 0);
	CreateIndexBuffer(indices, indicesSize, 101, 0);

	CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, L"g_samLinear");
	CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, L"g_samAnisotropic");
	CreateSamplerState(D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, D3D11_TEXTURE_ADDRESS_BORDER, L"g_samShadow");

}

void DXResourceManager::OnResize(ScreenInfo& sinfo)
{
	for (auto passes : m_RenderPasses)
	{
		passes.second->OnResize(sinfo, m_Device);
	}
}

//void DXResourceManager::ShaderDynamicLinkage(ComPtr<ID3D11ShaderReflection> ShaderReflector, std::shared_ptr<ShaderBase> ShaderBase)
//{
//	//다이나믹 링킹이란..?
//	//거대한 쉐이더를 만들때, 런타임시 적재할 레지스터를 동적으로 제어할 수 있는 것 같음.
//	//레지스터 공간 낭비하는 것보다 효율적이긴 할 것 같은데... 나는 우버쉐이더를 안쓸 것 같은데..?
//
//	//리플렉터 생성
//	UINT ShaderInterfaceNums = ShaderReflector->GetNumInterfaceSlots();
//}
//
//void DXResourceManager::CreateInputLayout(ComPtr<ID3DBlob> ShaderBlob, ComPtr<ID3D11ShaderReflection> ShaderReflector, std::shared_ptr<VertexShader> vertexShader)
//{
//	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc;
//
//	D3D11_SHADER_DESC ShaderDesc;
//	ShaderReflector->GetDesc(&ShaderDesc);
//
//	for (unsigned inputIndex = 0; inputIndex < ShaderDesc.InputParameters; inputIndex++)
//	{
//		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
//		ShaderReflector->GetInputParameterDesc(inputIndex, &paramDesc);
//
//		D3D11_INPUT_ELEMENT_DESC elementDesc;
//
//		elementDesc.SemanticName = paramDesc.SemanticName;
//		elementDesc.SemanticIndex = paramDesc.SemanticIndex; //동일한 시멘틱스를 사용하는 매개변수 구분
//		elementDesc.InputSlot = 0; //d3d11에 정의된 0~15값 input assembler
//		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; //편의를 위해 이걸로, 
//		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//		elementDesc.InstanceDataStepRate = 0;
//
//		//Mask which indicates which components of a register are used.
//
//		if (paramDesc.Mask == 1)
//		{
//			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
//		}
//		else if (paramDesc.Mask <= 3)
//		{
//			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
//		}
//		else if (paramDesc.Mask <= 7)
//		{
//			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
//		}
//		else if (paramDesc.Mask <= 15)
//		{
//			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
//			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//		}
//
//		InputLayoutDesc.push_back(elementDesc);
//	}
//
//	ComPtr<ID3D11InputLayout> InputLayout;
//
//	HR(m_Device->GetDevice()->CreateInputLayout(&InputLayoutDesc.front(), InputLayoutDesc.size(),
//		ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), InputLayout.GetAddressOf()));
//
//	vertexShader->SetInputLayout(InputLayout);
//}
//
//void DXResourceManager::CreateShaderBindBuffers(ComPtr<ID3D11ShaderReflection> ShaderReflector, std::shared_ptr<ShaderBase> ShaderBase)
//{
//	D3D11_SHADER_DESC shaderDesc;
//	ShaderReflector->GetDesc(&shaderDesc);
//
//	/*ShaderBase->CreateConstantBuffer();
//	ShaderBase->CreateTextureResourceBuffer();
//	ShaderBase->CreateSamplerBuffer();*/
//
//	//ShaderBase->SetConstantBufferSize(shaderDesc.ConstantBuffers);
//
//	for (int i = 0; i < shaderDesc.BoundResources; i++)
//	{
//		D3D11_SHADER_INPUT_BIND_DESC InputBindDesc;
//		ShaderReflector->GetResourceBindingDesc(i, &InputBindDesc);
//
//		//할일
//		//desc로 인자를 넣어서 만들 수 있도록 하기.
//		//sampler 함수 채우기
//
//		switch (InputBindDesc.Type)
//		{
//		case D3D_SIT_CBUFFER:
//			{
//				ID3D11ShaderReflectionConstantBuffer* BufferInfo = ShaderReflector->GetConstantBufferByName(InputBindDesc.Name);
//				CreateConstantBuffers(InputBindDesc.BindPoint, ShaderBase, BufferInfo);
//			}
//			break;
//		case D3D_SIT_TEXTURE:
//			{
//				CreateTextureResouceBuffer(InputBindDesc.BindPoint, ShaderBase, InputBindDesc.Name);
//				//srvRegisterSlot = resource_desc.BindPoint;
//				//_shaderResourceViewData[resource_desc.Name] = make_shared<ShaderResourceBuffer>(resource_desc.Name, srvRegisterSlot);
//			}
//			break;
//		case D3D_SIT_SAMPLER:
//			{
//				CreateSamplerBuffer(InputBindDesc.BindPoint, ShaderBase, InputBindDesc.Name);
//				//CreateSamplerBuffer(ShaderReflector, ShaderBase, InputBindDesc.Name);
//				//samplerRegisterSlot = resource_desc.BindPoint;
//				//_samplerStateData[resource_desc.Name] = make_shared<SamplerState>(resource_desc.Name, samplerRegisterSlot);
//			}
//			break;
//		default:
//			break;
//		}
//	}
//}
//
////
////void DXResourceManager::CreateConstantBuffers(UINT bindPoint, std::shared_ptr<ShaderBase> ShaderBase, ID3D11ShaderReflectionConstantBuffer* BufferInfo)
////{
////	//참조한 링크
////	//https://developer.nvidia.com/content/constant-buffers-without-constant-pain-0
////
////	D3D11_SHADER_BUFFER_DESC bufferDesc;
////
////	if (SUCCEEDED(BufferInfo->GetDesc(&bufferDesc)))
////	{
////		CONSTANTBUFFER_UPDATE_CLASSFICATION bufferUpdateType = CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_UNKNOWN;
////
////		if (strcmp(bufferDesc.Name, "cbPerObject") == 0)
////		{
////			bufferUpdateType = CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_PEROBJECT;
////		}
////		else if (strcmp(bufferDesc.Name, "cbPerFrame") == 0)
////		{
////			//이친구는 버퍼를 굳이 만들어줄 필요가 없다. 한번 적재할 버퍼 하나만 만들어주면 되니까..
////			if (m_PerFrameBuffer != nullptr)
////				return;
////
////			bufferUpdateType = CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_PERFRAME;
////		}
////		else
////		{
////			bufferUpdateType = CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_UNKNOWN;
////		}
////
////
////		//버퍼의 성분에 따라 들어가는 데이터가 다르니까, fixed는 고정돼있는 데이터 타입에따라 데이터를 집어 넣어준다. 
////		switch (bufferUpdateType)
////		{
////		case CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_PERFRAME:
////			{
////				m_PerFrameBuffer = std::make_shared<ConstantBuffer>();
////				m_PerFrameBuffer->Initialize(bufferDesc.Size, bindPoint, bufferDesc.Name, bufferUpdateType, m_Device);
////
////				for (int i = 0; i < bufferDesc.Variables; i++)
////				{
////					ID3D11ShaderReflectionVariable* BufferVariable = BufferInfo->GetVariableByIndex(i);
////					D3D11_SHADER_VARIABLE_DESC variableDesc;
////
////					BufferVariable->GetDesc(&variableDesc);
////
////					ConstantBufferLayout bufferLayout;
////
////					//ToDO.... 스트링비교를 효율적으로 할 수 있는 방법이 없을까?
////					if (strcmp(variableDesc.Name, "g_Camera"))
////					{
////						bufferLayout.m_Type = CONSTANTBUFFER_VARIABLE_DATATYPE::CONSTANTBUFFER_DATATYPE_TRANSFORM;
////					}
////					else
////					{
////						bufferLayout.m_Type = CONSTANTBUFFER_VARIABLE_DATATYPE::CONSTANTBUFFER_DATATYPE_CUSTOM;
////					}
////
////					bufferLayout.m_DataSize = variableDesc.Size;
////					bufferLayout.m_StartOffset = variableDesc.StartOffset;
////					bufferLayout.m_VariableName = variableDesc.Name;
////
////					m_PerFrameBuffer->PushConstantBufferLayout(bufferLayout);
////
////					//성분들을 규약할 수 있으려나..? 
////				}
////			}
////			break;
////		case CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_PEROBJECT:
////			{
////				std::shared_ptr<ConstantBuffer> buffer = ShaderBase->CreateConstantBuffer(bufferDesc.Size, bindPoint, bufferDesc.Name);
////				buffer->Initialize(bufferDesc.Size, bindPoint, bufferDesc.Name, bufferUpdateType, m_Device);
////
////				for (int i = 0; i < bufferDesc.Variables; i++)
////				{
////					ID3D11ShaderReflectionVariable* BufferVariable = BufferInfo->GetVariableByIndex(i);
////					D3D11_SHADER_VARIABLE_DESC variableDesc;
////
////					BufferVariable->GetDesc(&variableDesc);
////
////					ConstantBufferLayout bufferLayout;
////
////					//ToDO.... 스트링비교를 효율적으로 할 수 있는 방법이 없을까?
////					if (strcmp(variableDesc.Name, "g_Transform") == 0)
////					{
////						bufferLayout.m_Type = CONSTANTBUFFER_VARIABLE_DATATYPE::CONSTANTBUFFER_DATATYPE_TRANSFORM;
////					}
////					else if (strcmp(variableDesc.Name, "g_Material") == 0)
////					{
////						bufferLayout.m_Type = CONSTANTBUFFER_VARIABLE_DATATYPE::CONSTANTBUFFER_DATATYPE_MATERIAL;
////					}
////					else if (strcmp(variableDesc.Name, "g_Light") == 0)
////					{
////						bufferLayout.m_Type = CONSTANTBUFFER_VARIABLE_DATATYPE::CONSTANTBUFFER_DATATYPE_LIGHT;
////					}
////					else
////					{
////						bufferLayout.m_Type = CONSTANTBUFFER_VARIABLE_DATATYPE::CONSTANTBUFFER_DATATYPE_CUSTOM;
////					}
////
////					bufferLayout.m_DataSize = variableDesc.Size;
////					bufferLayout.m_StartOffset = variableDesc.StartOffset;
////					bufferLayout.m_VariableName = variableDesc.Name;
////					buffer->PushConstantBufferLayout(bufferLayout);
////
////					//성분들을 규약할 수 있으려나..? 
////				}
////			}
////			break;
////		case CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_UNKNOWN:
////			break;
////		default:
////			break;
////		}
////	}
////}
////
////void DXResourceManager::CreateTextureResouceBuffer(UINT bindPoint, std::shared_ptr<ShaderBase> ShaderBase, const std::string& BufferName)
////{
////	D3D11_SHADER_BUFFER_DESC bufferDesc;
////
////	std::shared_ptr<TextureResourceBuffer> buffer = ShaderBase->CreateTextureResourceBuffer(bindPoint, BufferName);
////}
////
////void DXResourceManager::CreateSamplerBuffer(UINT bindPoint, std::shared_ptr<ShaderBase> ShaderBase, const std::string& BufferName)
////{
////	D3D11_SHADER_BUFFER_DESC bufferDesc;
////
////	ShaderBase->CreateSamplerBuffer(bindPoint, BufferName);
////}
////
