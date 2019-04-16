#pragma once

#include <chrono>


class Timer
{
public:
	Timer();
	~Timer();

	float GetDeltaMilli();
	float GetMillisecondsElapsed();
	void Update();

private:
	std::chrono::high_resolution_clock::time_point m_startTime;
	std::chrono::high_resolution_clock::time_point m_previousTime;
	std::chrono::duration<float> m_deltaTime;

};

