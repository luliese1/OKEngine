#include "EnginePCH.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Transform.h"
#include "MeshRenderer.h"
#include "ColliderUpdater.h"

#include "ModelLoader.h"

SceneManager::SceneManager() : m_NowScene(nullptr), m_IsSceneInit(false), m_ScreenHeight(0), m_ScreenWidth(0)
{

}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize(int ScreenWidth, int ScreenHeight)
{
	ResizeScene(ScreenWidth, ScreenHeight);
}

void SceneManager::ResizeScene(int ScreenWidth, int ScreenHeight)
{
	m_ScreenWidth = ScreenWidth;
	m_ScreenHeight = ScreenHeight;
	if (m_NowScene != nullptr)
		m_NowScene->CalcProjection(ScreenWidth, ScreenHeight);
}

void SceneManager::Update(std::shared_ptr<IGraphicsEngine> graphics)
{
	if (!m_IsSceneInit)
	{
		if (m_NowScene != nullptr)
		{
			//씬로딩부분이랑 비슷한듯?
			for (auto loadData : m_NowScene->GetLoadResources())
			{
				ModelLoader::LoadModel(loadData, graphics);
			}
			for (auto loadObj : m_NowScene->GetLoadObject())
			{
				ModelLoader::AttachMeshs(loadObj.second ,loadObj.first);
			}
			m_NowScene->Initialize(m_ScreenWidth, m_ScreenHeight);
		}
		m_IsSceneInit = true;
	}

	if (m_NowScene != nullptr)
	{
		m_NowScene->Update(graphics);
	}
}

void SceneManager::Finalize()
{
	TransformUpdater::Finalize();
	ColliderUpdater::Finalize();
	MeshRenderer::Finalize();

	for (auto& scenes : m_SceneList)
	{
		scenes.second->Finalize();
		scenes.second.reset();
	}

	m_SceneList.clear();
}

bool SceneManager::CreateScene(int sceneNumber, std::shared_ptr<Scene> _scene)
{
	m_SceneList.insert({ sceneNumber, _scene });

	return true;
}

bool SceneManager::StartScene(int sceneNumber)
{
	auto result = m_SceneList.find(sceneNumber);

	if (result == m_SceneList.end())
	{
		return false;
	}
	else
	{
		m_NowScene = result->second;
		return true;
	}
}
