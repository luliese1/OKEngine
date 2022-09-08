#pragma once
#include <typeinfo>
#include <vector>

class ComponentBase;
class Scene;

class GameObject : public std::enable_shared_from_this<GameObject>
{
protected:
	GameObject();

	ObjectGUID m_ObjectID;
	std::wstring m_ObjectName;

	//���ӿ�����Ʈ�� ������Ʈ ����. 
	std::vector<std::shared_ptr<ComponentBase>> m_Components;

	std::weak_ptr<GameObject> m_Parent;
	std::vector<std::weak_ptr<GameObject>> m_Childs;

	std::weak_ptr<Scene> m_Scene;

	bool m_Enable;

	void AddChild(std::shared_ptr<GameObject> val)
	{
		m_Childs.push_back(val);
	};

public:
	GameObject(ObjectGUID key, const std::wstring& name);
	virtual ~GameObject();

	virtual void Init(); 
	virtual void Update();

	std::weak_ptr<GameObject> GetParent() const { return m_Parent; }
	void SetParent(std::shared_ptr<GameObject> val);

	std::weak_ptr<Scene> GetScene() const { return m_Scene; }
	void SetScene(std::shared_ptr<Scene> val) { m_Scene = val; }

	void DeleteChild(std::shared_ptr<GameObject> val);
	void DeleteChild(ObjectGUID& id);

	std::vector<std::weak_ptr<GameObject>>& GetChilds() { return m_Childs; }

	bool GetEnable();
	void SetEnable(bool val);

	std::wstring GetObjectName() const { return m_ObjectName; }
	ObjectGUID GetObjectID() const { return m_ObjectID; }
public:

	//������ ��εȴ�. T*�� ��ȯ������, template�� ����Ҷ����� �ڿ� *�� �ٿ�����.
	//�ϴ� ���������� �־��� T�� ���� ������(�ٿ��ش�� Ž��)
	template <typename T>
	std::shared_ptr<T> AddComponent();
	template <typename T>
	std::shared_ptr<T> AddComponent(std::shared_ptr<T>);
	template <typename T>
	std::shared_ptr<T> GetComponent();

};

// �ߺ��Ǵ� ������Ʈ�� ������ ó���� �غ���.
// ������Ʈ�� �־��־������� ��������Ʈ��, �ƴϸ� ���� ���� �־�����.

template <typename T>
std::shared_ptr<T> GameObject::AddComponent(std::shared_ptr<T> component)
{
	//if (!std::is_base_of<ComponentBase, T>::value)
	//{
	//	return nullptr
	//}
	
	std::shared_ptr<T> IsSameComp = this->GetComponent<T>();

	if (IsSameComp == nullptr)
	{
		std::shared_ptr<ComponentBase> input = std::dynamic_pointer_cast<ComponentBase>(component);
		if (input == nullptr)
		{
			return nullptr;
		}

		input->SetGameObject(shared_from_this());
		m_Components.push_back(input);

		return component;
	}
	else
	{
		return IsSameComp;
	}

	return nullptr;
}
template <typename T>
std::shared_ptr<T> GameObject::AddComponent()
{

	std::shared_ptr<T> IsSameComp = this->GetComponent<T>();

	if (IsSameComp == nullptr)
	{
		std::shared_ptr<T> input = std::make_shared<T>();
		input->SetGameObject(this);
		m_Components.push_back(input);

		return input;
	}
	else
	{
		return IsSameComp;
	}

	return nullptr;
}
template <typename T>
std::shared_ptr<T> GameObject::GetComponent()
{

	const type_info& info = typeid(T);

	for (auto comp : m_Components)
	{
		const type_info& targetinfo = typeid(*comp);

		if (info == targetinfo)
		{
			std::shared_ptr<T> output = std::static_pointer_cast<T>(comp);
			return output;
		}
	}

	return nullptr;
}

