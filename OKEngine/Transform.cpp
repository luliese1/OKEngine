#include "EnginePCH.h"
#include "Transform.h"

//#include "Scene.h"

using namespace DirectX;

void Transform::Init()
{

}

void Transform::Update()
{
	TransformUpdater::PushTransform(shared_from_this());
}

void Transform::Rotate(Vector3 val)
{
	m_Rotation += val;

}

void Transform::Translate(Vector3 val)
{
	m_Position += val;
}

void Transform::Scaling(Vector3 val)
{
	m_Scale += val;
}

DirectX::SimpleMath::Vector3 Transform::GetForward()
{
	return {m_WorldRotTM.m[2][0], m_WorldRotTM.m[2][1], m_WorldRotTM.m[2][2]};
}

DirectX::SimpleMath::Vector3 Transform::GetUp()
{
	return { m_WorldRotTM.m[1][0], m_WorldRotTM.m[1][1], m_WorldRotTM.m[1][2] };
}

DirectX::SimpleMath::Vector3 Transform::GetRight()
{
	return { m_WorldRotTM.m[0][0], m_WorldRotTM.m[0][1], m_WorldRotTM.m[0][2]};
}

Transform::Transform() : m_Scale( 1, 1, 1 ),
m_Rotation(),
m_Position(), m_IsCalculated(false),
m_LocalTM(), m_WorldTM()
{
}

Transform::~Transform()
{
}


void Transform::CalcLocalMatrix()
{
	m_LocalSclTM = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_LocalRotTM = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);;
	m_LocalPosTM = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	m_LocalTM = m_LocalSclTM * m_LocalRotTM * m_LocalPosTM;

	return;
}

DirectX::XMMATRIX Transform::CalcWorldMatrix()
{
	CalcLocalMatrix();
	std::weak_ptr<GameObject> parent = m_GameObject->GetParent();

	if (is_uninitialized(parent))
	{
		m_WorldTM = m_LocalTM;

	}
	else
	{
		std::shared_ptr<Transform> parentTransform = parent.lock().get()->GetComponent<Transform>();

		if (parentTransform->GetIsCalculated())
		{
			m_WorldTM = m_LocalTM * parentTransform->GetWorldTM();
		}
		else
		{
			m_WorldTM = m_LocalTM * parentTransform->CalcWorldMatrix();
		}
	}

	Vector3 WorldScl;
	Quaternion WorldRot;
	Vector3 WorldPos;

	m_WorldTM.Decompose(WorldScl, WorldRot, WorldPos);

	m_WorldSclTM = Matrix::CreateScale(WorldScl);
	m_WorldRotTM = Matrix::CreateFromQuaternion(WorldRot);
	m_WorldPosTM = Matrix::CreateTranslation(WorldPos);

	m_IsCalculated = true;
	return m_WorldTM;
}
