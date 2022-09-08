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
	XMMATRIX Scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	XMMATRIX Rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);;

	XMMATRIX TranslateMt = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	m_LocalTM = Scale * Rotation * TranslateMt;

	return;
}

DirectX::XMMATRIX Transform::CalcWorldMatrix()
{
	CalcLocalMatrix();
	std::weak_ptr<GameObject> parent = m_GameObject->GetParent();

	if (is_uninitialized(parent))
	{
		m_WorldTM = m_LocalTM;
		m_IsCalculated = true;
		return m_WorldTM;
	}
	else
	{
		std::shared_ptr<Transform> parentTransform = parent.lock().get()->GetComponent<Transform>();

		if (parentTransform->GetIsCalculated())
		{
			m_WorldTM = m_LocalTM * parentTransform->GetWorldTM();
			m_IsCalculated = true;
			return m_WorldTM;
		}
		else
		{
			m_WorldTM = m_LocalTM * parentTransform->CalcWorldMatrix();
			m_IsCalculated = true;
			return m_WorldTM;
		}
	}
}
