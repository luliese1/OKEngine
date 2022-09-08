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
				//�̹� �浹�ߴ��� �����Ѵ�
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

				//�浹������?
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