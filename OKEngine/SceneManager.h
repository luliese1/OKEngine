#pragma once

class Scene;
class IGraphicsEngine;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Initialize(int ScreenWidth, int ScreenHeight);
	void ResizeScene(int ScreenWidth, int ScreenHeight);
	void Update(std::shared_ptr<IGraphicsEngine>);
	void Finalize();

	std::shared_ptr<Scene> GetNowScene()
	{
		return m_NowScene;
	}
	bool CreateScene(int sceneNumber, std::shared_ptr<Scene> _scene);
	bool StartScene(int sceneNumber);

private:
	std::shared_ptr<Scene> m_NowScene;
	std::map<int, std::shared_ptr<Scene>> m_SceneList;

	int m_ScreenWidth;
	int m_ScreenHeight;

	bool m_IsSceneInit;
};

