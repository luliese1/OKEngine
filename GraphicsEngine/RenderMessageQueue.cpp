#include "pch.h"
#include "RenderMessageQueue.h"
#include "RenderMessage.h"

void RenderMessageQueue::PushMessage(std::shared_ptr<RenderMessage> msg)
{
	m_RenderMessageQueue.push(msg);
}

void RenderMessageQueue::Update()
{
	while(!m_RenderMessageQueue.empty())
	{
		std::shared_ptr<RenderMessage> msg;
		if (m_RenderMessageQueue.try_pop(msg))
		{
			msg->Execute();
		}
	}
}

bool RenderMessageQueue::IsEmpty()
{
	return m_RenderMessageQueue.empty();
}

void RenderMessageQueue::Finalize()
{
	m_RenderMessageQueue.clear();
}
