#include "EnginePCH.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : LightBase(LightBase::LightType::Directional)
{

}

DirectionalLight::~DirectionalLight()
{

}

DirectionalLightInfo DirectionalLight::GetDirectionalLightInfo() const
{
	return m_DirectionalLightInfo;
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

void DirectionalLight::SetDirection(Vector4 input)
{
	m_DirectionalLightInfo.Direction = input;
}
