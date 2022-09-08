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
	//그래픽스 엔진 생성.
	virtual int Initialize(GRAPHICSENGINE_ENGINE_DESC& GraphicsEngineDesc) abstract;
	//화면비 변환
	virtual void OnResize(int Width, int Height) abstract;
	//삭제
	virtual void Finalize() abstract;

	//메세지 생성 하는 Function
	//렌더시작
	virtual void StartRender() abstract;
	//PerFrameBuffer 채워넣기
	virtual void SetPerFrameConstantBuffer(void* ConstantBuffer) abstract;
	//백버퍼로 그리기
	virtual void Present() abstract;
	//렌더 및 ConstantBuffer 설정
	virtual void RenderObject(size_t MeshID, void* ConstantBuffer) abstract;
	//데이터 저장 및 갈아끼우기
	virtual void AddObject(size_t MeshID, MeshInfo meshInfo) abstract;
	//게임엔진에서도 삭제되었을떄 delete를 호출해서 삭제해주자.
	virtual void DeleteObject(size_t MeshID) abstract;
	
	//Pass
	//RunPostProcessing, 
	virtual void BindRenderPass(std::wstring passName) abstract;
	virtual void BindPostProcessPass(std::wstring passName) abstract;


	//PerObjectBuffer 채워넣기
	virtual void SetPerObjectConstantBuffer(void* ConstantBuffer) abstract;

	//BindTexture, 텍스쳐를 바인딩한다. 
	virtual void SetTexture(GRAPHICSENGINE_SHADER_RESOURCE_INPUT_LAYOUT* resourceInputLayout, UINT resourceInputLayoutCnt) abstract;
	
	//실행
	virtual void ExecutePass() abstract;

	//리소스 생성
	//버텍스, 인덱스 버퍼를 만든다
	virtual void CreateVertexBuffer(void* vertices, int vertexStructSize, int vertexCount, size_t MeshID, UINT subMeshID) abstract;
	virtual void CreateIndexBuffer(void* indices, int indexxCount, size_t MeshID, UINT subMeshID) abstract;

	//쉐이더 생성 -> 개별생성보단 이제 패스로 가자
	virtual void CreateRenderPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc) abstract;
	virtual void CreatePostProcessPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc) abstract;
	virtual void CreateShaderPass(const GRAPHICSENGINE_PASS_DESC& shaderPassDesc) abstract;
	
	//Texture
	virtual void CreateTexture(std::wstring path, size_t TextureID) abstract;
	virtual void CreateSamplerState(size_t SamplerID) abstract;

	//리소스 삭제
	virtual void DeleteMesh(size_t MeshID) abstract;
	virtual void DeleteVertexBuffer(size_t MeshID, UINT subMeshID) abstract;
	virtual void DeleteIndexBuffer(size_t MeshID, UINT subMeshID) abstract;
	virtual void DeleteTexture(size_t TextureID) abstract;
	virtual void DeleteSamplerState(size_t SamplerID) abstract;

	///리소스 체킹
	virtual bool CheckResourceData(size_t MeshID) abstract;
private:
};

