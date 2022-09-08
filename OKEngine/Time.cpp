#include "EnginePCH.h"
#include "Time.h"

__int64 Time::m_currTime = 0;
__int64 Time::m_prevTime = 0;
__int64 Time::m_countsPerSec = 0;
double Time::m_secondPerCount = 0;
int Time::m_FrameCount = 0;
double Time::m_FrameCountTime = 0;
int Time::m_FPS = 0;

double Time::m_deltaTime = 0.f;

void Time::Init()
{
	//���� Ÿ�̸��� ���� �ð�
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currTime);
	//���� Ÿ�̸��� ���ļ�(�ʴ簳��)
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_countsPerSec);

	m_secondPerCount = 1.0f / m_countsPerSec;
}


Time::Time()
{

}

Time::~Time()
{

}

void Time::Tick()
{
	//����ð��� ī���͸� �����´�
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currTime);

	//���������� ī���Ϳ� ����ð��� ī���͸� �� �� 1ȸ������ �ɸ� �ð��� ������.
	m_deltaTime = (m_currTime - m_prevTime) * m_secondPerCount;

	//���� �������� �غ���
	m_prevTime = m_currTime;

	//��Ű�� ��� ������ ���� �� ����
	if (m_deltaTime < 0.0)
	{
		m_deltaTime = 0.0f;
	}

	m_FrameCount++;
	m_FrameCountTime += m_deltaTime;

	// 1�ʸ� �Ѿ��..
	if (m_FrameCountTime >= 1.0f)
	{
		m_FPS = static_cast<int>(m_FrameCount / m_FrameCountTime);

		// fps ��� �� 0���� �ʱ�ȭ
		m_FrameCount = 0;
		m_FrameCountTime = 0.0f;
	}
}


