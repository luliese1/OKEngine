#include "pch.h"
#include "RenderMessage.h"
#include "RenderInfoManager.h"
#include "DXGraphicsEngine.h"

std::weak_ptr<DXGraphicsEngine> RenderMessage::m_GraphicsEngine;
std::weak_ptr<RenderInfoManager> RenderMessage::m_RenderInfoManager;

void RenderMessage::Release()
{
	m_GraphicsEngine.reset();
	m_RenderInfoManager.reset();
}

void RenderMessage_BeginRender::Execute()
{
	m_GraphicsEngine.lock().get()->StartRender_Excute();
}

void RenderMessage_EndRender::Execute()
{
	m_GraphicsEngine.lock().get()->Present_Execute();
}

void RenderMessage_DrawObject::Execute()
{
	m_GraphicsEngine.lock().get()->Render_Execute(m_ObjectGUID, m_ConstantBuffer);
}

void RenderMessage_SetConstantBuffer::Execute()
{

}

void RenderMessage_AddObject::Execute()
{
	m_RenderInfoManager.lock().get()->PushMeshInfo(m_ObjectGUID, m_ObjectInfo);
}

void RenderMessage_DeleteObject::Execute()
{

}
