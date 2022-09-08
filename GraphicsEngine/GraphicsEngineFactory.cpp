#include "pch.h"
#include "GraphicsEngineFactory.h"
#include "DXGraphicsEngine.h"


namespace GraphicsEngine
{
	std::shared_ptr<IGraphicsEngine> GraphicsEngineFactory::m_GraphicsEngine = nullptr;

	std::shared_ptr<IGraphicsEngine>  GraphicsEngineFactory::CreateDXGraphicsEngine()
	{
		if (m_GraphicsEngine == nullptr)
		{
			m_GraphicsEngine = std::make_shared<DXGraphicsEngine>();
		}
		else
		{
			return m_GraphicsEngine;
		}

		return m_GraphicsEngine;
	}

	bool GraphicsEngineFactory::ReleaseGraphicsEngine()
	{
		if (m_GraphicsEngine == nullptr)
		{
			return false;

		}
		else
		{
			m_GraphicsEngine.reset();
			return true;
		}
	}
}
 