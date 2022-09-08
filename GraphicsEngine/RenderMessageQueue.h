#pragma once


//���� �۵��ؾ��� 
//�����޼����� ���� ť
//�����޼����� �޼���Ǯ���� �����´�.

class RenderMessage;

class RenderMessageQueue
{
public:
	void PushMessage(std::shared_ptr<RenderMessage> msg);
	//���߿� ���������� �׷��Ƚ��� ������ �����带 ����� �����ְ�ʹ�.
	void Update();
	bool IsEmpty();

	void Finalize();

private:
	concurrency::concurrent_queue<std::shared_ptr<RenderMessage>> m_RenderMessageQueue;
};

