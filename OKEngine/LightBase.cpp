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

LightBase::~LightBase()
{
	
}

LightBase::LightBase(LightType type) : m_LightType(type)
{

}
