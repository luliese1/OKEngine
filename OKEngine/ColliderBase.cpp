 #include "EnginePCH.h"
#include "ColliderBase.h"
#include "Transform.h"
#include "ColliderUpdater.h"

void ColliderBase::Init()
{
	m_Transform = GetComponent<Transform>();
}

void ColliderBase::Update()
{
	ColliderUpdater::PushCollider(this);
}

void ColliderBase::AddCollidedObj(ColliderBase* collidedobj)
{
	m_CollidedObjs.push_back(collidedobj);
}

void ColliderBase::Clear()
{
	m_CollidedObjs.clear();
	m_IsCollided = false;
}

ColliderBase::ColliderBase() : m_Type(Collider_Type::BOX), m_IsCollided(false)
{
}

ColliderBase::~ColliderBase()
{
}
