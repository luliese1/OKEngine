#include "EnginePCH.h"
#include "TestScene.h"
#include "GameObject.h"
#include "CameraObject.h"
#include "Transform.h"
#include "Camera.h" 
#include "Mesh.h"
#include "DirectionalLight.h"
#include "SpotLIght.h"

void TestScene::Test()
{
	//씬테스트용 코드입니다 ^-^
	ObjectGUID genjiid = AddResource(L"./Resource/ASEFile/genji_max.ASE");
	ObjectGUID teapotid = AddResource(L"./Resource/ASEFile/teapot.ASE");

	std::shared_ptr<GameObject> camera = std::make_shared<CameraObject>();
	
	camera->GetComponent<Transform>()->SetPosition({ 0.0f, 10.0f, -5.0f });
	camera->GetComponent<Camera>()->SetLookAt({ 0.f,-1.f,0.f });

	m_ObjectList.emplace(1111, camera);
	m_CameraComp.emplace_back(camera->GetComponent<Camera>());

	std::shared_ptr<GameObject> Box = MakeGameObject(L"Box1");
	{
		MeshInfo tempMeshInfo;
		tempMeshInfo.m_MeshId = 101;
		tempMeshInfo.m_PassName = L"TexStatic";

		tempMeshInfo.m_TextureID[0] = 101;
		tempMeshInfo.m_TextureCnt = 1;

		tempMeshInfo.m_SamplerId = 101;

		Box->AddComponent<Mesh>()->SetMeshInfo(tempMeshInfo);
		Box->GetComponent<Transform>()->SetScale({ 5.0f, 0.1f, 5.f });
	}
	std::shared_ptr<GameObject> Genji = MakeGameObject(L"Genji");
	Genji->GetComponent<Transform>()->SetScale({ 5.0f, 0.3f, 5.f });
	Genji->GetComponent<Transform>()->Translate({ 0.0f,1.0f,0.f });
	LoadObject(Genji, genjiid);



	std::shared_ptr<GameObject> Lightobj = MakeGameObject(L"Light1");
	{
		auto lightComp = Lightobj->AddComponent<DirectionalLight>();
		lightComp->SetAmbient({ 0.7f, 0.7f, 0.7f, 1.f });
		lightComp->SetDiffuse({ 0.7f, 0.7f, 0.7f, 1.f });
		lightComp->SetSpecular({ 0.5f, 0.5f, 0.5f, 1.f });
		lightComp->SetDirection({ 0.0f, -0.5f, 0.0f, 0.f });
	}


}
