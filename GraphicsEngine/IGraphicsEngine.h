#pragma once
#include "dllDefine.h"

struct CameraInfo;
struct RenderInfo;
struct MeshInfo;

/// <summary>
/// 
/// 2022-06-08
/// GraphicsEngine Interface
/// 
/// </summary>

class GRAPHICENGINELIB_API IGraphicsEngine
{
public:
	//�׷��Ƚ� ���� ����.
	virtual int Initialize(GRAPHICSENGINE_ENGINE_DESC& GraphicsEngineDesc) abstract;
	//ȭ��� ��ȯ
	virtual void OnResize(int Width, int Height) abstract;
	//����
	virtual void Finalize() abstract;

	//�޼��� ���� �ϴ� Function
	//��������
	virtual void StartRender() abstract;
	//PerFrameBuffer ä���ֱ�
	virtual void SetPerFrameConstantBuffer(void* ConstantBuffer) abstract;
	//����۷� �׸���
	virtual void Present() abstract;
	//���� �� ConstantBuffer ����
	virtual void RenderObject(size_t MeshID, void* ConstantBuffer) abstract;
	//������ ���� �� ���Ƴ����
	virtual void AddObject(size_t MeshID, MeshInfo meshInfo) abstract;
	//���ӿ��������� �����Ǿ����� delete�� ȣ���ؼ� ����������.
	virtual void DeleteObject(size_t MeshID) abstract;
	
	//Pass
	//RunPostProcessing, 
	virtual void BindRenderPass(std::wstring passName) abstract;
	virtual void BindPostProcessPass(std::wstring passName) abstract;


	//PerObjectBuffer ä���ֱ�
	virtual void SetPerObjectConstantBuffer(void* ConstantBuffer) abstract;

	//BindTexture, �ؽ��ĸ� ���ε��Ѵ�. 
	virtual void SetTexture(GRAPHICSENGINE_SHADER_RESOURCE_INPUT_LAYOUT* resourceInputLayout, UINT resourceInputLayoutCnt) abstract;
	
	//����
	virtual void ExecutePass() abstract;

	//���ҽ� ����
	//���ؽ�, �ε��� ���۸� �����
	virtual void CreateVertexBuffer(void* vertices, int vertexStructSize, int vertexCount, size_t MeshID, UINT subMeshID) abstract;
	virtual void CreateIndexBuffer(void* indices, int indexxCount, size_t MeshID, UINT subMeshID) abstract;

	//���̴� ���� -> ������������ ���� �н��� ����
	virtual void CreateRenderPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc) abstract;
	virtual void CreatePostProcessPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc) abstract;
	virtual void CreateShaderPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc) abstract;
	
	//Texture
	virtual void CreateTexture(std::wstring path, size_t TextureID) abstract;
	virtual void CreateSamplerState(size_t SamplerID) abstract;

	//���ҽ� ����
	virtual void DeleteMesh(size_t MeshID) abstract;
	virtual void DeleteVertexBuffer(size_t MeshID, UINT subMeshID) abstract;
	virtual void DeleteIndexBuffer(size_t MeshID, UINT subMeshID) abstract;
	virtual void DeleteTexture(size_t TextureID) abstract;
	virtual void DeleteSamplerState(size_t SamplerID) abstract;

	///���ҽ� üŷ
	virtual bool CheckResourceData(size_t MeshID) abstract;
private:
};

