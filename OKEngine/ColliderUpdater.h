#pragma once

class ColliderBase;

class ColliderUpdater
{
public:
	static void Update();
	static void PushCollider(ColliderBase* pCollider);
	static void Finalize();

private:
	ColliderUpdater();
	~ColliderUpdater();

	static std::vector<ColliderBase*> m_ColliderBases;
};

