#include "EnginePCH.h"
#include "LightBase.h"
#include "MeshRenderer.h"
#include "Transform.h"

void LightBase::Init()
{
	m_Transform = GetComponent<Transform>();
}

void LightBase::Update()
{
	MeshRenderer::PushLight(shared_from_this());
}

Matrix LightBase::GetProjectionMatrix()
{
	//return Matrix::Identity;
	return Matrix();
}

Matrix LightBase::GetViewMatrix()
{
	//return Matrix::Identity;
	return Matrix();
}

LightBase::~LightBase()
{
	
}

LightBase::LightBase(LightType type) : m_LightType(type)
{

}
