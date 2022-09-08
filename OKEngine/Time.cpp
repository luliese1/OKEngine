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
	//성능 타이머의 현재 시간
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currTime);
	//성능 타이머의 주파수(초당개수)
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
	//현재시간의 카운터를 가져온다
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currTime);

	//전프레임의 카운터와 현재시간의 카운터를 뺀 후 1회진동에 걸린 시간을 곱해줌.
	m_deltaTime = (m_currTime - m_prevTime) * m_secondPerCount;

	//다음 프레임을 준비함
	m_prevTime = m_currTime;

	//엉키는 경우 음수가 나올 수 있음
	if (m_deltaTime < 0.0)
	{
		m_deltaTime = 0.0f;
	}

	m_FrameCount++;
	m_FrameCountTime += m_deltaTime;

	// 1초를 넘어가면..
	if (m_FrameCountTime >= 1.0f)
	{
		m_FPS = static_cast<int>(m_FrameCount / m_FrameCountTime);

		// fps 계산 후 0으로 초기화
		m_FrameCount = 0;
		m_FrameCountTime = 0.0f;
	}
}


