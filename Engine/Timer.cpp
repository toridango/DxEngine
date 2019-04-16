#include "Timer.h"



Timer::Timer()
{
	m_startTime = std::chrono::high_resolution_clock::now();
	m_previousTime = m_startTime;
}


Timer::~Timer()
{
}



float Timer::GetDeltaMilli()
{
	return m_deltaTime.count();
}

float Timer::GetMillisecondsElapsed()
{
	auto elapsed = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - m_startTime);
	return elapsed.count();
}


void Timer::Update()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration<float, std::milli>(now - m_previousTime);
	m_deltaTime = elapsed;
	m_previousTime = now;
}