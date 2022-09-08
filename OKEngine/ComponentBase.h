#pragma once
#include "GameObject.h"

//16바이트 정렬
class ComponentBase : public std::enable_shared_from_this<ComponentBase>
{
public:
	virtual void Init() abstract;
	virtual void Update() abstract;
	//virtual void LateUpdate();

	GameObject* GetGameObject() const { return m_GameObject; }
	void SetGameObject(GameObject* val) { m_GameObject = val; }
	virtual ~ComponentBase();

protected:
	ComponentBase();

	GameObject* m_GameObject;

public:

	template <typename T>
	std::shared_ptr<T> GetComponent();

	template <typename T>
	std::shared_ptr<T> AddComponent();

};

template <typename T>
std::shared_ptr<T> ComponentBase::GetComponent()
{
	if (m_GameObject != nullptr)
	{
		return m_GameObject->GetComponent<T>();
	}

	return nullptr;
}

template <typename T>
std::shared_ptr<T> ComponentBase::AddComponent()
{

	if (m_GameObject != nullptr)
	{
		return m_GameObject->AddComponent<T>();
	}

	return nullptr;
}

