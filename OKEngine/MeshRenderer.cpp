#include "EnginePCH.h"
#include "MeshRenderer.h"
#include "ComponentBase.h"
#include "Transform.h"
#include "Mesh.h"
#include "UI.h"
#include "Camera.h"
#include "IGraphicsEngine.h"

#include "LightBase.h"
#include "DirectionalLight.h"
#include "SpotLIght.h"
#include "PointLight.h"


std::queue<std::shared_ptr<Mesh>> MeshRenderer::m_MeshQueue;
std::queue<std::shared_ptr<UI>> MeshRenderer::m_UIQueue;
std::queue< std::shared_ptr<LightBase>> MeshRenderer::m_LightQueue;

void MeshRenderer::Render(std::shared_ptr<Camera> camera, RenderInfo renderinfo, std::shared_ptr<IGraphicsEngine> Graphics)
{
	std::vector<ObjectGUID> staticMeshGUIDs;
	std::vector<StaticMesh> staticMeshInfo;
	staticMeshInfo.reserve(m_MeshQueue.size());


	PerFrameBuffer perFrameBuffer;

	//�ø�
	//������Ʈ ����Ұ͵� ����
	//�޽������� ���Ѱ� ������ Update

	//���߿� Frame������ ������ �����Ҹ�����������.

	//��ģ������ �ø��� ���������... ����Ѵ�

	while (!m_MeshQueue.empty())
	{
		//�̱����� �³�..? �ǹ��� ���
		//�׷��Ƚ��� ���ҽ��� �ִ��� üũ�Ѵ�.
		std::shared_ptr<Mesh> mesh = m_MeshQueue.front();

		StaticMesh tempmesh;

		tempmesh.World = mesh->GetComponent<Transform>()->GetWorldTM();
		tempmesh.WorldInvTranspose = tempmesh.World.Invert().Transpose();

		ObjectGUID objID = mesh->GetGameObject()->GetObjectID();
		staticMeshGUIDs.push_back(objID);
		staticMeshInfo.emplace_back(tempmesh);

		Graphics->AddObject(objID, mesh->GetMeshInfo());

		m_MeshQueue.pop();
		//MeshInfo tempMeshInfo = mesh->GetMeshInfo();
		//meshInfos.emplace_back(tempMeshInfo);
	}

	//������ PerFrameBuffer Bind
	Graphics->StartRender();
	LoadLightInfos(perFrameBuffer.m_LightInfo);

	perFrameBuffer.m_cameraInfo.m_ProjMatrix = camera->GetProjectionMatrix();
	perFrameBuffer.m_cameraInfo.m_ViewMatrix = camera->GetViewMatrix();
	perFrameBuffer.m_cameraInfo.m_ViewProjectionMatrix = perFrameBuffer.m_cameraInfo.m_ViewMatrix * perFrameBuffer.m_cameraInfo.m_ProjMatrix;
	perFrameBuffer.m_cameraInfo.m_ViewProjectionMatrixInverseTranspose = perFrameBuffer.m_cameraInfo.m_ViewProjectionMatrix.Invert();

	Graphics->SetPerFrameConstantBuffer(&perFrameBuffer);

	Graphics->BindRenderPass(L"Shadow");
	Graphics->RenderObjects(staticMeshGUIDs.data(), staticMeshInfo.data(), sizeof(StaticMesh), staticMeshGUIDs.size());

	Graphics->BindRenderPass(L"Basic");
	Graphics->RenderObjects(staticMeshGUIDs.data(), staticMeshInfo.data(), sizeof(StaticMesh), staticMeshGUIDs.size());

	Graphics->BindPostProcessPass(L"light");
	GRAPHICSENGINE_SHADER_RESOURCE_INPUT_LAYOUT lightInput[] =
	{
		{SHADER_RESOURCE_SOURCE::PASS, L"Basic", 0},
		{SHADER_RESOURCE_SOURCE::PASS, L"Basic", 1},
		{SHADER_RESOURCE_SOURCE::PASS, L"Basic", 3},
		{SHADER_RESOURCE_SOURCE::PASSDEPTH, L"Shadow", 0},
	};
	Graphics->SetPerObjectConstantBuffer(nullptr);
	Graphics->SetTextures(lightInput, 4);
	Graphics->ExecutePass();

	Graphics->Present();
}


void MeshRenderer::PushMesh(std::shared_ptr<ComponentBase> comp)
{
	std::shared_ptr<Mesh> pMesh = std::dynamic_pointer_cast<Mesh>(comp);
	if (pMesh != nullptr)
		m_MeshQueue.push(pMesh);
	else
		assert("MeshRenderer : PushMesh");
}

void MeshRenderer::PushUI(std::shared_ptr<ComponentBase> comp)
{
	std::shared_ptr<UI> pUI = std::dynamic_pointer_cast<UI>(comp);
	if (pUI != nullptr)
		m_UIQueue.push(pUI);
	else
		assert("MeshRenderer : PushUI");

}

void MeshRenderer::PushLight(std::shared_ptr<ComponentBase> comp)
{
	std::shared_ptr<LightBase> pLight = std::dynamic_pointer_cast<LightBase>(comp);
	if (pLight != nullptr)
		m_LightQueue.push(pLight);
	else
		assert("MeshRenderer : PushLight");
}

void MeshRenderer::LoadLightInfos(LightInfo& Lightinfos)
{
	Lightinfos.DirectionalLightCnt = 0;
	Lightinfos.PointLightCnt = 0;
	Lightinfos.SpotLightCnt = 0;

	while (!m_LightQueue.empty())
	{
		auto lightInfo = m_LightQueue.front();

		switch (lightInfo->GetLightType())
		{
		case LightBase::LightType::Directional:
			{
				std::shared_ptr<DirectionalLight> direction = std::dynamic_pointer_cast<DirectionalLight>(lightInfo);
				if (Lightinfos.DirectionalLightCnt > 3)
				{
					assert("directional light�� �ִ� 3������ �����մϴ�.");
				}
				else
				{
					Lightinfos.DirectInfos[Lightinfos.DirectionalLightCnt] = direction->GetDirectionalLightInfo();
					Lightinfos.DirectionalLightCnt++;
				}
			}
			break;
		case LightBase::LightType::Spot:
			{
				std::shared_ptr<SpotLIght> spot = std::dynamic_pointer_cast<SpotLIght>(lightInfo);
				if (Lightinfos.SpotLightCnt > 10)
				{
					assert("spot light�� �ִ� 10������ �����մϴ�.");
				}
				else
				{
					Lightinfos.SpotInfos[Lightinfos.SpotLightCnt] = spot->GetSpotLightInfo();
					auto info = spot->GetComponent<Transform>()->GetWorldTM();
					Lightinfos.SpotInfos[Lightinfos.SpotLightCnt].PositionRange.x = info.m[3][0];
					Lightinfos.SpotInfos[Lightinfos.SpotLightCnt].PositionRange.y = info.m[3][1];
					Lightinfos.SpotInfos[Lightinfos.SpotLightCnt].PositionRange.z = info.m[3][2];
					Lightinfos.SpotLightCnt++;
				}
			}
			break;
		case LightBase::LightType::Point:
			{
				std::shared_ptr<PointLight> point = std::dynamic_pointer_cast<PointLight>(lightInfo);
				if (Lightinfos.PointLightCnt > 10)
				{
					assert("point light�� �ִ� 10������ �����մϴ�.");
				}
				else
				{
					Lightinfos.PointInfos[Lightinfos.PointLightCnt] = point->GetPointLIghtInfo();
					auto info = point->GetComponent<Transform>()->GetWorldTM();
					Lightinfos.PointInfos[Lightinfos.PointLightCnt].PositionRange.x = info.m[3][0];
					Lightinfos.PointInfos[Lightinfos.PointLightCnt].PositionRange.y = info.m[3][1];
					Lightinfos.PointInfos[Lightinfos.PointLightCnt].PositionRange.z = info.m[3][2];
					Lightinfos.PointLightCnt++;
				}
			}
			break;
		default:
			break;
		}

		m_LightQueue.pop();
	}
}

void MeshRenderer::Finalize()
{
	while (!m_MeshQueue.empty())
	{
		m_MeshQueue.pop();
	}

	while (!m_UIQueue.empty())
	{
		m_UIQueue.pop();
	}
}
