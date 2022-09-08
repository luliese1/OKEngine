#include "EnginePCH.h"
#include "GameEngineFactory.h"
#include "OKEngine.h"

namespace GameEngine
{
	std::shared_ptr<IGameEngine> GameEngineFactory::m_GameEngine = nullptr;

	std::shared_ptr<IGameEngine> GameEngineFactory::CreateOKEngine()
	{
		m_GameEngine = std::make_shared<OKEngine>();

		return m_GameEngine;
	}

	bool GameEngineFactory::ReleaseOKEngine()
	{
		if (m_GameEngine == nullptr)
		{
			return false;
		}
		else
		{
			m_GameEngine->Finalize();
			m_GameEngine.reset();
			return true;
		}
	}
}