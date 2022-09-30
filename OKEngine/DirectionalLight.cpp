#include "EnginePCH.h"
#include "DirectionalLight.h"
#include "Transform.h"

DirectionalLight::DirectionalLight() : LightBase(LightBase::LightType::Directional)
{

}

DirectionalLight::~DirectionalLight()
{

}

DirectionalLightInfo DirectionalLight::GetDirectionalLightInfo()
{
	m_DirectionalLightInfo.LightProjectionTM = GetViewMatrix() * GetProjectionMatrix();
	return m_DirectionalLightInfo;
}

Matrix DirectionalLight::GetProjectionMatrix()
{
	return DirectX::XMMatrixOrthographicLH(10, 10, 0.f, 20);
}

Matrix DirectionalLight::GetViewMatrix()
{
	return m_Transform.lock()->GetWorldTM();
}

void DirectionalLight::SetAmbient(Vector4 input)
{
	m_DirectionalLightInfo.Ambient = input;
}

void DirectionalLight::SetDiffuse(Vector4 input)
{
	m_DirectionalLightInfo.Diffuse = input;
}

void DirectionalLight::SetSpecular(Vector4 input)
{
	m_DirectionalLightInfo.Specular = input;
}

void DirectionalLight::SetDirection(Vector3 input)
{
	m_DirectionalLightInfo.Direction = {input.x, input.y, input.z, 0.f};

	std::shared_ptr<Transform> transform = GetComponent<Transform>();

	Vector3 rot = input;
	Vector3 vec = DirectX::XMLoadFloat3(&rot);

	Vector4 lookVec = DirectX::XMVector3Normalize(vec);
	Vector4 upVec = { 0.f , 1.f, 0.f, 0.f };

	Vector4 rightVec = DirectX::XMVector3Cross(upVec, lookVec);
	upVec = DirectX::XMVector3Cross(lookVec, rightVec);


	float pitch = (float)asin(-lookVec.y);
	float yaw = (float)atan2(lookVec.x, lookVec.z);
	float roll = (float)atan2(rightVec.y, upVec.y);

	transform->SetRotation({ pitch, yaw, roll });

}

