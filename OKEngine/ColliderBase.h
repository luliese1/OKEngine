#pragma once
#include "ComponentBase.h"
#include "AlignedAllocationPolicy.h"
//���� �ڽ��� �ϴ� ������ 

//�⺻������ �ݶ��̴��� ������ �͵��� ������

class Transform;

enum class Collider_Type
{
	SPHERE,
	PLANE,
	CAPSULE,
	BOX
};

class ColliderBase : public ComponentBase, public AlignedAllocationPolicy<16>
{
public:
	virtual void Init() override;
	virtual void Update() override;

	Collider_Type GetType() const { return m_Type; }
	void SetType(Collider_Type val) { m_Type = val; }

	bool GetIsCollided() const { return m_IsCollided; }
	void SetIsCollided(bool val) { m_IsCollided = val; }

	virtual bool IsCollided(ColliderBase* pCollider) abstract;

	void AddCollidedObj(ColliderBase* pOhterCollider);
	std::vector<ColliderBase*>& GetCollidesObjs() { return m_CollidedObjs; }
	virtual void Clear();

	std::shared_ptr<Transform> GetTransform() const { return m_Transform; }

protected:
	ColliderBase();
	virtual ~ColliderBase();

	//�̹������� �浹�� �ݶ��̴���?
	std::vector<ColliderBase*> m_CollidedObjs;
	Collider_Type m_Type;

	std::shared_ptr<Transform> m_Transform;

	bool m_IsCollided;
};

