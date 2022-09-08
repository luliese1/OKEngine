#pragma once
#include "IGraphicsEngine.h"
#include "dllDefine.h"
#include <memory>

namespace GraphicsEngine
{
	class GraphicsEngineFactory
	{
	public:
		static GRAPHICENGINELIB_API std::shared_ptr<IGraphicsEngine> CreateDXGraphicsEngine();
		static GRAPHICENGINELIB_API bool ReleaseGraphicsEngine();

	private:
		static std::shared_ptr<IGraphicsEngine> m_GraphicsEngine;
	};
}
