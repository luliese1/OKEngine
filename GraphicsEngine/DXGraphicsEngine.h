#pragma once
#pragma once
#include "IGraphicsEngine.h"

/// <summary>
/// 
/// 윈도우의 DirectX11을 기준으로..
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
	//그래픽스 엔진 생성.
	virtual int Initialize(GRAPHICSENGINE_ENGINE_DESC& GraphicsEngineDesc) override;
	//화면비 변환
	virtual void OnResize(int Width, int Height) override;
	//삭제
	virtual void Finalize() override;

	//메세지 생성 하는 Function
	//렌더시작
	virtual void StartRender() override;
	//PerFrameBuffer 채워넣기
	virtual void SetPerFrameConstantBuffer(void* ConstantBuffer) override;
	//백버퍼로 그리기
	virtual void Present() override;
	//렌더 및 ConstantBuffer 설정
	virtual void RenderObject(size_t MeshID, void* ConstantBuffer) override;
	virtual void RenderObjects(size_t* MeshID, void* ConstantBuffer, UINT ConstantBufSize, UINT objectCnt) override;

	//데이터 저장 및 갈아끼우기
	virtual void AddObject(size_t MeshID, MeshInfo meshInfo) override;
	//게임엔진에서도 삭제되었을떄 delete를 호출해서 삭제해주자.
	virtual void DeleteObject(size_t MeshID) override;

	////메세지 가지고있는거 실행해주기
	//virtual void FlushMessage() override;

	//Pass
	//RunPostProcessing, 
	virtual void BindRenderPass(std::wstring passName) override;
	virtual void BindPostProcessPass(std::wstring passName) override;
	//PerObjectBuffer 채워넣기
	virtual void SetPerObjectConstantBuffer(void* ConstantBuffer) override;
	//BindTexture, 텍스쳐를 바인딩한다. 
	virtual void SetTextures(GRAPHICSENGINE_SHADER_RESOURCE_INPUT_LAYOUT* resourceInputLayout, UINT resourceInputLayoutCnt) override;
	//실행
	virtual void ExecutePass() override;

	//데이터 생성함수
	virtual void CreateShaderPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc) override;
	virtual void CreatePostProcessPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc) override;
	virtual void CreateRenderPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc) override;

	virtual void CreateVertexBuffer(void* vertices, int vertexStructSize, int vertexCount, size_t MeshID, UINT subMeshID) override;
	virtual void CreateIndexBuffer(void* indices, int indexxCount, size_t MeshID, UINT subMeshID) override;
	virtual void CreateTexture(std::wstring path, size_t TextureID) override;
	virtual void CreateSamplerState(size_t SamplerID) override;
	virtual void CreateRasterizerState(std::wstring RasterizerID, const GRAPHICSENGINE_RASTERIZER_DESC& rasterizerDesc) override;

	//데이터 삭제함수
	virtual void DeleteMesh(size_t MeshID) override;
	virtual void DeleteVertexBuffer(size_t MeshID, UINT subMeshID) override;
	virtual void DeleteIndexBuffer(size_t MeshID, UINT subMeshID) override;
	virtual void DeleteTexture(size_t TextureID) override;
	virtual void DeleteSamplerState(size_t SamplerID) override;

	///리소스 체킹
	virtual bool CheckResourceData(size_t MeshID) override;

public:
	//TODO Rasterizer, sampler..... 등 만들것
	void CreateRenderState();
	//실질적인 Render_Execute 코드들
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

	//나중에 이친구들도 따로 Renderer로 뺴고싶지만,... 
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<SwapChain> m_SwapChain;

	std::shared_ptr<RenderPass> m_BindingRenderPass;
	std::shared_ptr<PassBase> m_BindingPostProcessPass;

	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
};

