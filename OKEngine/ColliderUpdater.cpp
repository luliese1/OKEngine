#include "EnginePCH.h"
#include "ColliderUpdater.h"
#include "ColliderBase.h"

void ColliderUpdater::Update()
{
	for (auto& colliderComp : m_ColliderBases)
	{
		colliderComp->Clear();
	}

	for (auto& colliderComp : m_ColliderBases)
	{
		for (auto& others : m_ColliderBases)
		{
			if (colliderComp != others)
			{
				//이미 충돌했는지 검증한다
				bool alreadCollided = false;
				for (auto& id : colliderComp->GetCollidesObjs())
				{
					if (id == others)
					{
						alreadCollided = true;
					}
				}
				if (alreadCollided)
				{
					continue;
				}

				//충돌했으면?
				if (colliderComp->IsCollided(others))
				{
					colliderComp->AddCollidedObj(others);
					colliderComp->SetIsCollided(true);
					others->AddCollidedObj(colliderComp);
					others->SetIsCollided(true);
				}
			}
		}
	}

	m_ColliderBases.clear();
}

void ColliderUpdater::PushCollider(ColliderBase* pCollider)
{
	m_ColliderBases.push_back(pCollider);
}

void ColliderUpdater::Finalize()
{
	m_ColliderBases.clear();
	m_ColliderBases.shrink_to_fit();
}

std::vector<ColliderBase*> ColliderUpdater::m_ColliderBases;

ColliderUpdater::ColliderUpdater()
{

}

ColliderUpdater::~ColliderUpdater()
{

}