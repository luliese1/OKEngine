#pragma once
#include "IGameEngine.h"
#include "dllDefine.h"
#include <memory.h>

namespace GameEngine
{
	class GameEngineFactory
	{
	public:
		static OKENGINE_API std::shared_ptr<IGameEngine> CreateOKEngine();
		static OKENGINE_API bool ReleaseOKEngine();

	private:
		static std::shared_ptr<IGameEngine> m_GameEngine;
	};

}
