#pragma once

class Camera;
class MeshRenderer;
class GameObject;
class SkyBox;
class UIObject;
class IGraphicsEngine;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
	Scene();
	virtual ~Scene();

	void Initialize(int ScreenWidth, int ScreenHeight);
	void Update(std::shared_ptr<IGraphicsEngine> graphics);
	void Finalize();

	void CalcProjection(int Width, int Height);

	std::shared_ptr<GameObject>  MakeGameObject(const std::wstring& ObjectName);
	std::shared_ptr<UIObject>  MakeUIObject(const std::wstring& ObjectName);

	ObjectGUID AddResource(std::wstring path);
	void LoadObject(std::shared_ptr<GameObject> target, ObjectGUID id){ m_LoadObject.emplace_back(target, id);}

	std::unordered_map<ObjectGUID, std::shared_ptr<GameObject>>& GetObjectList() {
		return m_ObjectList;
	}

	//오브젝트들을 순회하며 찾습니다..
	std::shared_ptr<GameObject> FindObjectByName(const std::wstring& name);
	//오브젝트에 key값을 통해 바로 접근이 가능합니다.
	std::shared_ptr<GameObject> FindObjectByObjectID(ObjectGUID id);

	void AddCamera(std::shared_ptr<Camera> comp)
	{
		m_CameraComp.push_back(comp);
	};
	int GetCameraID() const { return m_CameraID; }
	std::shared_ptr<SkyBox> GetSkyBox() const { return m_SkyBox; }
	void SetSkyBox(std::shared_ptr<SkyBox> val) { m_SkyBox = val; }

	std::vector<ModelManageInfo>& GetLoadResources() { return m_LoadResources; }
	std::vector<std::pair<std::shared_ptr<GameObject>, ObjectGUID>>& GetLoadObject() { return m_LoadObject; }


protected:
	std::vector<std::shared_ptr<Camera>> m_CameraComp;
	std::unordered_map<ObjectGUID, std::shared_ptr<GameObject>> m_ObjectList;
	std::shared_ptr<SkyBox> m_SkyBox;

	int m_CameraID;
	int m_CubemapID;

	//TODO 
	//나중에 GUID 발급으로 바꾸자. 일단 씬에서 대충대충~
	int m_localidxcreate = 0;

	///
	//씬데이터
	
	//불려와야할 데이터들
	std::vector<ModelManageInfo> m_LoadResources;
	//언제해줘야함?
	std::vector<std::pair<std::shared_ptr<GameObject>, ObjectGUID>> m_LoadObject;

};

