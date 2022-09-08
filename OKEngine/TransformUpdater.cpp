#include "EnginePCH.h"
#include "TransformUpdater.h"

#include "ComponentBase.h"
#include "Transform.h"
#include "Camera.h"

TransformUpdater::TransformUpdater()
{

}

TransformUpdater::~TransformUpdater()
{

}

std::queue<std::shared_ptr<Transform>> TransformUpdater::m_TransformComps;
std::queue<std::shared_ptr<Camera>> TransformUpdater::m_Cameras;

void TransformUpdater::Update()
{
	for (size_t i = 0; i < m_Cameras.size(); i++)
	{
		m_Cameras.front()->CalcViewMatrix();

		m_Cameras.pop();
	}

	for (size_t i = 0; i < m_TransformComps.size(); i++)
	{
		m_TransformComps.front()->CalcWorldMatrix();
		m_TransformComps.push(m_TransformComps.front());
		m_TransformComps.pop();
	}

	for (size_t i = 0; i < m_TransformComps.size(); i++)
	{
		m_TransformComps.front()->SetIsCalculated(false);
		m_TransformComps.pop();
	}
}

void TransformUpdater::PushTransform(std::shared_ptr<ComponentBase> pTransform)
{
	m_TransformComps.push(std::dynamic_pointer_cast<Transform>(pTransform));

}
void TransformUpdater::PushCamera(std::shared_ptr<ComponentBase> pCamera)
{
	m_Cameras.push(std::dynamic_pointer_cast<Camera>(pCamera));
}

void TransformUpdater::Finalize()
{
	while (!m_TransformComps.empty())
	{
		m_TransformComps.pop();
	}

	while (!m_Cameras.empty())
	{
		m_Cameras.pop();
	}
}
