#include "EnginePCH.h"
#include "GameObject.h"

#include "EnginePCH.h"
#include "GameObject.h"
#include "ComponentBase.h"



GameObject::GameObject() : m_Enable(true), m_Parent(), m_Scene()
{
}

GameObject::GameObject(ObjectGUID key, const std::wstring& name) : m_Enable(true), m_Parent(), m_Scene()
{
	m_ObjectID = key;
	m_ObjectName = name;
}

GameObject::~GameObject()
{

	for (auto& comps : m_Components)
	{
		comps.reset();
	}

	for (auto& child : m_Childs)
	{
		child.reset();
		//delete(child);
	}
}

void GameObject::Init()
{
	for (const auto& comp : m_Components)
	{
		comp->Init();
	}
}

void GameObject::Update()
{
	if (m_Enable)
	{
		for (const auto& comp : m_Components)
		{
			comp->Update();
		}
	}
}

void GameObject::SetParent(std::shared_ptr<GameObject> val)
{
	if (is_uninitialized(m_Parent))
	{
		m_Parent = val;
		m_Parent.lock().get()->AddChild(shared_from_this());
	}
	else
	{
		m_Parent.lock().get()->DeleteChild(shared_from_this());
		m_Parent = val;
		m_Parent.lock().get()->AddChild(shared_from_this());
	}
}

void GameObject::DeleteChild(std::shared_ptr<GameObject> val)
{
	if (m_Childs.size() > 0)
	{
		for (auto pChild = m_Childs.begin(); pChild != m_Childs.end(); pChild++)
		{
			if (pChild->lock().get()->GetObjectID() == val->GetObjectID())
			{
				m_Childs.erase(pChild);
				break;
			}
		}
	}
}

void GameObject::DeleteChild(ObjectGUID& id)
{
	if (m_Childs.size() > 0)
	{
		for (auto pChild = m_Childs.begin(); pChild != m_Childs.end(); pChild++)
		{
			if (pChild->lock().get()->GetObjectID() == id)
			{
				m_Childs.erase(pChild);
				break;
			}
		}
	}
}

bool GameObject::GetEnable()
{
	return m_Enable;
}

void GameObject::SetEnable(bool val)
{
	m_Enable = val;

	for (auto& childs : m_Childs)
	{
		childs.lock().get()->SetEnable(val);
	}

}
