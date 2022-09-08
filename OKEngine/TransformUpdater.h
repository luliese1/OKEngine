#pragma once
/// <summary>
/// Ʈ�������� �������� �༮�̴�.
/// </summary>

class ComponentBase;
class Transform;
class Camera;

class TransformUpdater
{
public:
	static void Update();
	static void PushTransform(std::shared_ptr<ComponentBase> pTransform);
	static void PushCamera(std::shared_ptr<ComponentBase> pCamera);

	static void Finalize();

private:
	TransformUpdater();
	~TransformUpdater();

	static std::queue<std::shared_ptr<Transform>> m_TransformComps;
	static std::queue<std::shared_ptr<Camera>> m_Cameras;
};