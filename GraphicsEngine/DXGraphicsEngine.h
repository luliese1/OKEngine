#pragma once
#pragma once
#include "IGraphicsEngine.h"

/// <summary>
/// 
/// �������� DirectX11�� ��������..
/// 
/// </summary>

class DXResourceManager;
class RenderInfoManager;

class Device;
class RenderTargetView;
class SwapChain;
class DepthStencilView;
class ConstantBuffer;

class RenderPass;
class VertexShader;
class PixelShader;
class PassBase;

namespace DirectX
{
	class SpriteBatch;
}

class DXGraphicsEngine : public IGraphicsEngine, public std::enable_shared_from_this<DXGraphicsEngine>
{
public:
	DXGraphicsEngine();
	virtual ~DXGraphicsEngine();
	//�׷��Ƚ� ���� ����.
	virtual int Initialize(GRAPHICSENGINE_ENGINE_DESC& GraphicsEngineDesc) override;
	//ȭ��� ��ȯ
	virtual void OnResize(int Width, int Height) override;
	//����
	virtual void Finalize() override;

	//�޼��� ���� �ϴ� Function
	//��������
	virtual void StartRender() override;
	//PerFrameBuffer ä���ֱ�
	virtual void SetPerFrameConstantBuffer(void* ConstantBuffer) override;
	//����۷� �׸���
	virtual void Present() override;
	//���� �� ConstantBuffer ����
	virtual void RenderObject(size_t MeshID, void* ConstantBuffer) override;
	virtual void RenderObjects(size_t* MeshID, void* ConstantBuffer, UINT ConstantBufSize, UINT objectCnt) override;

	//������ ���� �� ���Ƴ����
	virtual void AddObject(size_t MeshID, MeshInfo meshInfo) override;
	//���ӿ��������� �����Ǿ����� delete�� ȣ���ؼ� ����������.
	virtual void DeleteObject(size_t MeshID) override;

	////�޼��� �������ִ°� �������ֱ�
	//virtual void FlushMessage() override;

	//Pass
	//RunPostProcessing, 
	virtual void BindRenderPass(std::wstring passName) override;
	virtual void BindPostProcessPass(std::wstring passName) override;
	//PerObjectBuffer ä���ֱ�
	virtual void SetPerObjectConstantBuffer(void* ConstantBuffer) override;
	//BindTexture, �ؽ��ĸ� ���ε��Ѵ�. 
	virtual void SetTextures(GRAPHICSENGINE_SHADER_RESOURCE_INPUT_LAYOUT* resourceInputLayout, UINT resourceInputLayoutCnt) override;
	//����
	virtual void ExecutePass() override;

	//������ �����Լ�
	virtual void CreateShaderPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc) override;
	virtual void CreatePostProcessPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc) override;
	virtual void CreateRenderPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc) override;

	virtual void CreateVertexBuffer(void* vertices, int vertexStructSize, int vertexCount, size_t MeshID, UINT subMeshID) override;
	virtual void CreateIndexBuffer(void* indices, int indexxCount, size_t MeshID, UINT subMeshID) override;
	virtual void CreateTexture(std::wstring path, size_t TextureID) override;
	virtual void CreateSamplerState(size_t SamplerID) override;
	virtual void CreateRasterizerState(std::wstring RasterizerID, const GRAPHICSENGINE_RASTERIZER_DESC& rasterizerDesc) override;

	//������ �����Լ�
	virtual void DeleteMesh(size_t MeshID) override;
	virtual void DeleteVertexBuffer(size_t MeshID, UINT subMeshID) override;
	virtual void DeleteIndexBuffer(size_t MeshID, UINT subMeshID) override;
	virtual void DeleteTexture(size_t TextureID) override;
	virtual void DeleteSamplerState(size_t SamplerID) override;

	///���ҽ� üŷ
	virtual bool CheckResourceData(size_t MeshID) override;

public:
	//TODO Rasterizer, sampler..... �� �����
	void CreateRenderState();
	//�������� Render_Execute �ڵ��
	void StartRender_Excute();
	//void Render_Execute(CameraInfo& cameraInfo, RenderInfo& renderInfo, MeshInfo* meshInfo, UINT meshInfoSize);
	void Render_Execute(size_t MeshID, void* bufferSrc);
	void Present_Execute();

protected:
	void SetViewport(float width, float height);

private:

	HINSTANCE m_hInst;
	HWND m_hWnd;

	ScreenInfo m_ScreenInfo;

	std::shared_ptr<DXResourceManager> m_ResourceManager;
	std::shared_ptr<RenderInfoManager> m_RenderInfoManager;

	//���߿� ��ģ���鵵 ���� Renderer�� ���������,... 
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<SwapChain> m_SwapChain;

	std::shared_ptr<RenderPass> m_BindingRenderPass;
	std::shared_ptr<PassBase> m_BindingPostProcessPass;

	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
};

