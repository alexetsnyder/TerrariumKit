#include "Time.h"

namespace SysTK
{
	Time::Time()
	{
		_deltaTime = std::chrono::duration<double>(0.0);
		_lastFrame = std::chrono::high_resolution_clock::now();
	}

	Time::~Time()
	{
	}

	double Time::deltaTime() const
	{
		return _deltaTime.count();
	}

	void Time::update()
	{
		std::chrono::high_resolution_clock::time_point currentFrame = std::chrono::high_resolution_clock::now();
		_deltaTime = currentFrame - _lastFrame;
		_lastFrame = currentFrame;
	}
}

