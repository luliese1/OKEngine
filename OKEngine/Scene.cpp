#include "EnginePCH.h"
#include "Scene.h"
#include "GameObject.h"

#include "Camera.h"

#include "Transform.h"
#include "MeshRenderer.h"
#include "ColliderUpdater.h"

#include "ResourceHashTable.h"

Scene::Scene()
	:
	m_SkyBox(nullptr), m_CameraComp(0), m_CameraID(0), m_CubemapID(0)
{
}

Scene::~Scene()
{
}

void Scene::Initialize(int ScreenWidth, int ScreenHeight)
{
	//TransformUpdater::Update();

	for (auto& Objs : m_ObjectList)
	{
		Objs.second->Init();
	}

}

void Scene::Update(std::shared_ptr<IGraphicsEngine> graphics)
{
	for (auto& Objs : m_ObjectList)
	{
		Objs.second->Update();
	}

	TransformUpdater::Update();
	ColliderUpdater::Update();

	if (m_CameraComp.size() > 0)
		MeshRenderer::Render(m_CameraComp[m_CameraID], RenderInfo(), graphics);
}

void Scene::Finalize()
{
	for (auto& Objs : m_ObjectList)
	{
		Objs.second.reset();
	}
	m_ObjectList.clear();
	m_CameraComp.clear();
	m_SkyBox.reset();
}

void Scene::CalcProjection(int Width, int Height)
{
	for (auto& iter : m_CameraComp)
	{
		iter->CalcProjection(Width, Height);
	}
}

std::shared_ptr<GameObject> Scene::MakeGameObject(const std::wstring& ObjectName)
{
	std::shared_ptr<GameObject> temp = std::make_shared<GameObject>(m_localidxcreate, ObjectName);
	temp->AddComponent<Transform>();
	temp->SetScene(shared_from_this());

	m_ObjectList.emplace(m_localidxcreate, temp);
	m_localidxcreate++;
	return temp;
}

std::shared_ptr<UIObject> Scene::MakeUIObject(const std::wstring& ObjectName)
{
	//UIObject* temp = new UIObject;
	//temp->AddComponent<RectTransform>();
	//temp->SetScene(this);
	//m_ObjectList.push_back(temp);
	m_localidxcreate++;
	//return temp;
	return nullptr;
}


ObjectGUID Scene::AddResource(std::wstring path)
{
	std::wstring name;
	
	//이름자르기.
	for (auto iter = path.end() - 1; iter != path.begin(); iter--)
	{
		if (*iter == '/')
		{
			name.assign(iter + 1, path.end());
			break;
		}
	}

	ObjectGUID guid = ResourceHashTable::GetHashKey(name);

	ModelManageInfo tempInfo;

	tempInfo.m_name = name;
	tempInfo.m_path = path;
	tempInfo.m_GUID = guid;

	m_LoadResources.push_back(tempInfo);



	return guid;
}

std::shared_ptr<GameObject> Scene::FindObjectByName(const std::wstring& name)
{
	for (auto& Obj : m_ObjectList)
	{
		if (Obj.second->GetObjectName() == name)
		{
			return Obj.second;
		}
	}
	return nullptr;
}

std::shared_ptr<GameObject> Scene::FindObjectByObjectID(ObjectGUID id)
{
	auto target = m_ObjectList.find(id);

	return (target == m_ObjectList.end())? nullptr : target->second;
}

