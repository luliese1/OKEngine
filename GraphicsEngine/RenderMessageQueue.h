#pragma once


//현재 작동해야할 
//렌더메세지에 대한 큐
//렌더메세지는 메세지풀에서 가져온다.

class RenderMessage;

class RenderMessageQueue
{
public:
	void PushMessage(std::shared_ptr<RenderMessage> msg);
	//나중엔 병렬적으로 그래픽스만 돌리는 스레드를 만들어 돌려주고싶다.
	void Update();
	bool IsEmpty();

	void Finalize();

private:
	concurrency::concurrent_queue<std::shared_ptr<RenderMessage>> m_RenderMessageQueue;
};

