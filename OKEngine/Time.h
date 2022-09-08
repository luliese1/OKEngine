#pragma once

class Time
{
public:
	static __int64 m_currTime;
	static __int64 m_prevTime;
	static __int64 m_countsPerSec;

	static double m_secondPerCount;
	static double m_deltaTime;

	static int m_FrameCount;
	static double m_FrameCountTime;
	static int m_FPS;

	static void Init();


private:
	Time();
	~Time();

public:
	static void Tick();
};

