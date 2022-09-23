#pragma once
#include "IResourceManager.h"

class Device;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class ShaderBase;
class Texture;
class SamplerState;

class PassBase;

class PixelShader;
class VertexShader;

/// <summary>
/// 
/// DX �������� ���� ���ҽ����� ������ ���ҽ� �Ŵ����Դϴ�.
/// 
/// </summary>

class DXResourceManager : public IResourceManager
{
public:
	void Initialize(std::shared_ptr<Device>& device);
	void Finalize();

	std::shared_ptr<PixelShader> GetPixelShader(size_t);
	std::shared_ptr<VertexShader>  GetVertexShader(size_t);
	void DeleteVertexShader(size_t ObjecID);
	void DeletePixelShader(size_t ObjecID);

	//�ε������� �����ּ�, �ε��� ����, ���� guid, �޽����� �ε�����ȣ
	void CreateIndexBuffer(void* indices, int indexCount, size_t ObjecID, UINT index);
	void CreateVertexBuffer(void* vertices, int vertexStructSize, int vertexCount, size_t ObjecID, UINT index);
	void CreateTexture(tstring path, size_t ObjecID);
	void CreateMaterial(size_t ObjecID);
	void CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE textureOutside, std::wstring ObjecID);
	void CreateRasterizerState(std::wstring RasterizerID, const GRAPHICSENGINE_RASTERIZER_DESC& rasterizerDesc);

	void CreateRenderPass(ScreenInfo& sinfo, const GRAPHICSENGINE_PASS_DESC& shaderPassDesc);
	void CreatePostProcessPass(ScreenInfo& sinfo, const GRAPHICSENGINE_PASS_DESC& shaderPassDesc);
	void CreateShaderPass(ScreenInfo& sinfo, const GRAPHICSENGINE_PASS_DESC& shaderPassDesc);

	void DeleteMesh(size_t ObjecID);
	void DeleteIndexBuffer(size_t ObjecID, UINT subObjectID);
	void DeleteVertexBuffer(size_t ObjecID, UINT subObjectID);
	void DeleteTexture(size_t ObjecID);
	void DeleteMaterial(size_t ObjecID);
	void DeleteSamplerState(std::wstring ObjecID);

	std::shared_ptr<VertexBuffer> GetVertexBuffer(size_t, UINT);
	std::shared_ptr<IndexBuffer> GetIndexBuffer(size_t, UINT);
	std::shared_ptr<Material> GetMaterial(size_t);
	std::shared_ptr<Texture> GetTexture(size_t);
	std::shared_ptr<PassBase> GetPass(std::wstring);
	std::shared_ptr<SamplerState> GetSamplerState(std::wstring);
	ComPtr<ID3D11RasterizerState> GetRaterizerState(std::wstring);

	void TestFunction();

	void OnResize(ScreenInfo& sinfo);
private:
	//void ShaderDynamicLinkage(ComPtr<ID3D11ShaderReflection> ShaderReflector, std::shared_ptr<ShaderBase> ShaderBase);
	//void CreateInputLayout(ComPtr<ID3DBlob> ShaderBlob, ComPtr<ID3D11ShaderReflection> ShaderReflector, std::shared_ptr<VertexShader> vertexShader);
	//void CreateShaderBindBuffers(ComPtr<ID3D11ShaderReflection> ShaderReflector, std::shared_ptr<ShaderBase> ShaderBase);
	//constant buffer ����

private:
	ComPtr<ID3D11ClassLinkage> m_ShaderDynamicLinkage;

	//vb, Ib
	struct Mesh
	{
		//�� �޽��� �������� ���ؽ��� �ε��� ���۰� �� �� �����Ƿ�.. 
		std::map<UINT, std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::map<UINT, std::shared_ptr<IndexBuffer>> m_IndexBuffers;
	};

	std::unordered_map<size_t, std::shared_ptr<Mesh>> m_Meshs;

	//Texture, Material
	std::unordered_map<size_t, std::shared_ptr<Texture>> m_Textures;
	std::unordered_map<size_t, std::shared_ptr<Material>> m_Materials;

	//PASS
	std::unordered_map<std::wstring, std::shared_ptr<PassBase>> m_RenderPasses;
	//PSShader, VSShader
	std::unordered_map<size_t, std::shared_ptr<PixelShader>> m_PixelShaders;
	std::unordered_map<size_t, std::shared_ptr<VertexShader>> m_VertexShaders;

	//States
	std::unordered_map<std::wstring, std::shared_ptr<SamplerState>> m_SamplerState;
	std::unordered_map<std::wstring, ComPtr<ID3D11RasterizerState>> m_RasterizerState;

	std::shared_ptr<Device> m_Device;
};

