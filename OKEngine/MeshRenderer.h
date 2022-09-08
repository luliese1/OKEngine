#pragma once

#define MAX_DIRECTIONAL_LIGHT_CNT 3
#define MAX_POINT_LIGHT_CNT 10
#define MAX_SPOT_LIGHT_CNT 10

//메쉬 렌더 큐느낌의 클래스가 될 것이다.
//정렬이 필요하면 이곳에서,
//오클루전이 필요하면 이곳에서,
//그래픽 엔진간의 소통도 여기서

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

