#pragma once

#define MAX_DIRECTIONAL_LIGHT_CNT 3
#define MAX_POINT_LIGHT_CNT 10
#define MAX_SPOT_LIGHT_CNT 10

//�޽� ���� ť������ Ŭ������ �� ���̴�.
//������ �ʿ��ϸ� �̰�����,
//��Ŭ������ �ʿ��ϸ� �̰�����,
//�׷��� �������� ���뵵 ���⼭

class Mesh;
class UI;
class LightBase;

class ComponentBase;
class IGraphicsEngine;
class Camera;
struct RenderInfo;

class MeshRenderer
{
public:
	struct LightInfo
	{
		UINT DirectionalLightCnt;
		UINT PointLightCnt;
		UINT SpotLightCnt;
		UINT pad;
		DirectionalLightInfo DirectInfos[MAX_DIRECTIONAL_LIGHT_CNT];
		PointLightInfo PointInfos[MAX_POINT_LIGHT_CNT];
		SpotLightInfo SpotInfos[MAX_SPOT_LIGHT_CNT];
	};

public:
	static void Render(std::shared_ptr<Camera> camera, RenderInfo infos ,std::shared_ptr<IGraphicsEngine> Graphics);

	static void PushMesh(std::shared_ptr<ComponentBase>);
	static void PushUI(std::shared_ptr<ComponentBase>);
	static void PushLight(std::shared_ptr<ComponentBase>);

	static void LoadLightInfos();

	static void Finalize();
private:
	static std::queue<std::shared_ptr<Mesh>> m_MeshQueue;
	static std::queue< std::shared_ptr<UI>> m_UIQueue;
	static std::queue< std::shared_ptr<LightBase>> m_LightQueue;
	static LightInfo m_LightInfo;

	//std::queue<ColliderBase*> m_Colliders;
};

