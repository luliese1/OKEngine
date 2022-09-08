#include "EnginePCH.h"
#include "MeshRenderer.h"
#include "ModelLoader.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Scene.h"
#include "Transform.h"

Mesh::Mesh() : m_IsChanged(true)
{

}

Mesh::~Mesh()
{

}

void Mesh::Update()
{
	MeshRenderer::PushMesh(shared_from_this());
}

void Mesh::Init()
{
}

