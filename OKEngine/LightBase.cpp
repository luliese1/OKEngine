#include "EnginePCH.h"
#include "LightBase.h"
#include "MeshRenderer.h"

void LightBase::Init()
{

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
