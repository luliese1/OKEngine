#include "EnginePCH.h"
#include "OKEngine.h"
#include "GraphicsEngineFactory.h"

#include "TestScene.h"

bool OKEngine::Initialize(long instance, long handle, int ScreenWidth, int ScreenHeight)
{
	m_GraphicsEngine = GraphicsEngine::GraphicsEngineFactory::CreateDXGraphicsEngine();

	GRAPHICSENGINE_ENGINE_DESC EngineDesc;
	// --------------------------  Graphics Initialize ----------------------------

	EngineDesc.handle = handle;
	EngineDesc.instance = instance;
	EngineDesc.ScreenWidth = ScreenWidth;
	EngineDesc.ScreenHeight = ScreenHeight;

	m_GraphicsEngine->Initialize(EngineDesc);

	// --------------------------  RenderPass  ----------------------------

	GRAPHICSENGINE_PASS_DESC BasicPass;
	BasicPass.m_PassName = L"Basic";

	// -------------------------- Mesh Shader ----------------------------

	std::vector<std::vector<GRAPHICSENGINE_SHADER_MACRO_DESC>> staticmacros =
	{
	 {{"USING_ALBEDO", NULL}},
	 {{"USING_ALBEDO", NULL}, {"USING_NORMALMAP", NULL}}
	};

	std::wstring vsMesh = L"./Shader/VetexShader_Mesh.hlsl";
	std::wstring psMesh = L"./Shader/PixelShader_Mesh.hlsl";

	GRAPHICSENGINE_SHADER_DESC BaseShaderDesc[] =
	{
		{L"NoneTexStatic", vsMesh, psMesh, nullptr, 0 },
		{L"TexStatic", vsMesh, psMesh , &staticmacros[0].front(), (UINT)staticmacros[0].size()},
		{L"TexNormalStatic", vsMesh, psMesh, &staticmacros[1].front(), (UINT)staticmacros[1].size()},
	};

	GRAPHICSENGINE_RENDER_TARGET_OUTPUT_LAYOUT BaseShaderOutput[] =
	{
		{SHADER_RENDER_TARGET::PASS, L""},
		{SHADER_RENDER_TARGET::PASS, L""},
		{SHADER_RENDER_TARGET::PASS, L""},
		{SHADER_RENDER_TARGET::PASS, L""},
		{SHADER_RENDER_TARGET::PASS, L""}
	};

	BasicPass.m_ShaderDesc = BaseShaderDesc;
	BasicPass.m_ShaderCount = 3;
	BasicPass.m_RenderTargetLayout = BaseShaderOutput;
	BasicPass.m_OutputTexturesCount = 4;

	m_GraphicsEngine->CreateRenderPass(BasicPass);

	// -------------------------- Shadow Shader ----------------------------

	GRAPHICSENGINE_PASS_DESC ShadowPass;
	ShadowPass.m_PassName = L"Shadow";

	std::wstring psShadow = L"./Shader/PixelShader_Shadow.hlsl";

	GRAPHICSENGINE_SHADER_DESC ShadowShaderDesc[] =
	{
		{L"NoneTexStatic", vsMesh, psShadow, nullptr, 0 },
		{L"TexStatic", vsMesh, psShadow , &staticmacros[0].front(), (UINT)staticmacros[0].size()},
		{L"TexNormalStatic", vsMesh, psShadow, &staticmacros[1].front(), (UINT)staticmacros[1].size()},
	};

	ShadowPass.m_ShaderDesc = ShadowShaderDesc;
	ShadowPass.m_ShaderCount = 3;

	GRAPHICSENGINE_RENDER_TARGET_OUTPUT_LAYOUT shadowShaderOutput[] =
	{
		{SHADER_RENDER_TARGET::PASS, L""}
	};

	ShadowPass.m_RenderTargetLayout = shadowShaderOutput;
	ShadowPass.m_OutputTexturesCount = 1;

	m_GraphicsEngine->CreateRenderPass(ShadowPass);

	// -------------------------- Deferred Lighting ----------------------------

	GRAPHICSENGINE_PASS_DESC LightingPass;

	LightingPass.m_PassName = L"light";

	GRAPHICSENGINE_SHADER_DESC LightingShaderDesc[] =
	{
		{L"Lighting", L"", L"./Shader/Light.hlsl", nullptr, 0}
	};

	LightingPass.m_ShaderDesc = LightingShaderDesc;
	LightingPass.m_ShaderCount = 1;
	LightingPass.m_OutputTexturesCount = 1;

	GRAPHICSENGINE_RENDER_TARGET_OUTPUT_LAYOUT LightRenderTargetLayouts[] =
	{
		{SHADER_RENDER_TARGET::BACKBUFFER, L""}
	};

	LightingPass.m_RenderTargetLayout = LightRenderTargetLayouts;
	m_GraphicsEngine->CreatePostProcessPass(LightingPass);

	// -------------------------- Post Processing ----------------------------


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_SceneManager.Initialize(ScreenWidth, ScreenHeight);

	Input::Init();
	Time::Init();

	std::shared_ptr<TestScene> Ts = std::make_shared<TestScene>();

	Ts->Test();

	m_SceneManager.CreateScene(101, Ts);
	m_SceneManager.StartScene(101);

	return true;
}

void OKEngine::OnResize(int Width, int Height)
{
	m_GraphicsEngine->OnResize(Width, Height);
	m_SceneManager.ResizeScene(Width, Height);

}

void OKEngine::SetMousePos(LPARAM lParam)
{
	Input::SetMousePos(lParam);
}

void OKEngine::SetMouseWheel(WPARAM lParam)
{
	Input::SetMouseWheel(lParam);
}

bool OKEngine::GetMouseInWindow()
{
	return Input::bMouseInWindow;
}

void OKEngine::SetMouseInWindow(bool flag)
{
	Input::bMouseInWindow = flag;
}

void OKEngine::Loop()
{
	Input::Tick();
	Time::Tick();

	//Scene Loop
	m_SceneManager.Update(m_GraphicsEngine);

}

void OKEngine::Finalize()
{
	Input::Finalize();
	ModelLoader::Finalize();
	m_SceneManager.Finalize();
	m_GraphicsEngine->Finalize();
	m_GraphicsEngine.reset();
	GraphicsEngine::GraphicsEngineFactory::ReleaseGraphicsEngine();
}

