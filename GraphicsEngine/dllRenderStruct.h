#pragma once
#include "dllDefine.h"

#define MAXIMUM_SHADER_RESOURCE_PER_MESH 8
#define MAXIMUM_RENDERTARGET_TEXTURE_PER_MESH 8
#define MAXIMUM_SAMPLER_RESOURCE_PER_MESH 4


struct GRAPHICENGINELIB_API RenderInfo
{
	UINT m_CubeMapID;
};

struct GRAPHICENGINELIB_API MeshInfo
{
	MeshInfo() : m_MeshId(0), m_subMeshId(0), m_MaterialID(0), m_TextureID(), m_TextureCnt(0), m_PassName(){}
	MeshInfo& operator =(const MeshInfo& info) = default;

	size_t  m_MeshId;
	UINT8 m_subMeshId;

	////�ؽ��� ������ ��¼��.. ��.. �������� �������ٱ�
	size_t  m_MaterialID;

	size_t  m_TextureID[MAXIMUM_SHADER_RESOURCE_PER_MESH];
	UINT8 m_TextureCnt;

	std::wstring m_PassName;
};

struct GRAPHICENGINELIB_API LightInfo
{
	enum class LightType
	{
		LIGHTTYPE_DIRECTION,
		LightType_POINT,
		LIGHTTYPE_SPOT
	};
	LightType m_LightType;

	DirectX::XMMATRIX m_Diffuse;
	DirectX::XMMATRIX m_Ambient;
	DirectX::XMMATRIX m_Specular;
	DirectX::XMFLOAT3 Direction;
	float Range;
	DirectX::XMFLOAT3 Position;
	float Spot;
	DirectX::XMFLOAT3 Att;
	float Pad;
};

// �ڿ��� ��ó
enum class GRAPHICENGINELIB_API SHADER_RESOURCE_SOURCE
{
	BACKBUFFER,
	PASS,
	PASSDEPTH,
	TEXTURE
};

enum class GRAPHICENGINELIB_API SHADER_RENDER_TARGET
{
	BACKBUFFER,
	PASS,
	OTHERPASS
};


//DIRECT11�� �����ϰ� �ǵ� NULL NULL�� �־�����Ѵ�.
struct GRAPHICENGINELIB_API GRAPHICSENGINE_SHADER_MACRO_DESC
{
	LPCSTR Name;
	LPCSTR Definition;
};
// BASICPASS or Texture�� ����Ѵٸ�, m_ResourceName �� �ʿ����� �ʴ�.
struct GRAPHICENGINELIB_API GRAPHICSENGINE_SHADER_RESOURCE_INPUT_LAYOUT
{
	SHADER_RESOURCE_SOURCE m_ResourceSource;
	std::wstring m_ResourceName;
	size_t m_ResourceIndex;
};

struct GRAPHICENGINELIB_API GRAPHICSENGINE_RENDER_TARGET_OUTPUT_LAYOUT
{
	SHADER_RENDER_TARGET m_ResourceSource;

	std::wstring m_passName;
};

struct GRAPHICENGINELIB_API GRAPHICSENGINE_SHADER_DESC
{
	GRAPHICSENGINE_SHADER_DESC(const std::wstring& shaderSetName, const std::wstring& VSPath, const std::wstring& PSPath, GRAPHICSENGINE_SHADER_MACRO_DESC* ShaderMacro, UINT ShaderMacroCnt);

	std::wstring m_ShaderSetName;

	std::wstring m_VertexShaderPath;
	std::wstring m_PixelShaderPath;

	//���̴� ��ũ�δ� DX�� �Ȱ��� �������� ���� �˾Ƽ� �� ������־ �־�����.
	GRAPHICSENGINE_SHADER_MACRO_DESC* m_ShaderMacro;
	UINT m_ShaderMacroCnt;
};
struct GRAPHICENGINELIB_API GRAPHICSENGINE_PASS_DESC
{
	std::wstring m_PassName;

	//MRT�� ���� ������(RenderPass)�� ���ε� �ؾ��Ѵ�. 
	//�ٸ��н����� ���� �ؽ��ķ� �۾��ϴ� ���� �ϴ� �����Ѵ�.. basic pass������ ��������.
	//UINT m_InputTexturesCount;  // ���Ǵ� SRV �ؽ����� �����̴�.. �˾Ƽ� �� ����...
	
	GRAPHICSENGINE_RENDER_TARGET_OUTPUT_LAYOUT* m_RenderTargetLayout;

	int m_TextureWidth = -1; // 0���� �����ô� ��ũ��������� 
	int m_TextureHeight = -1; // 0���� �����ô� ��ũ���������

	//��������� ������ �ؽ��ĵ�
	UINT m_OutputTexturesCount; // ���Ǵ� RTV �ؽ����� �����̴�... �ִ� 8������ �����ϴ�

	GRAPHICSENGINE_SHADER_DESC* m_ShaderDesc;
	UINT m_ShaderCount;
};

struct GRAPHICENGINELIB_API GRAPHICSENGINE_RASTERIZER_DESC
{
	enum class eFillMode 
	{
		WIRE = 2,
		SOLID = 3
	};

	enum class eCullMode 
	{
		CULL_NONE = 1,
		CULL_FRONT = 2,
		CULL_BACK = 3
	};

	eFillMode m_FillMode = eFillMode::WIRE;
	eCullMode m_CullMode = eCullMode::CULL_NONE;

	int m_DepthBias = 0;
	float m_DepthBiasClamp = 0.000f;
	float m_SlopeScaledDepthBias = 0.000f;

	bool m_DepthClipEnable = true;
	bool m_ScissorEnable = true;
	bool m_MultiSampleEnable = true;
	bool m_AntialiasedLineEnable = true;
};

//������ �����Ҷ� �⺻������ �ʿ��� �����͵��� ����� ���Ƴ���.
struct GRAPHICENGINELIB_API GRAPHICSENGINE_ENGINE_DESC
{
	long instance;
	long handle;

	int ScreenWidth;
	int ScreenHeight;

	//GRAPHICSENGINE_PASS_DESC* m_BasicPass;
	//UINT m_MultiRenderTargetCnt; //�ּ� 1~D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT(8)����
};

