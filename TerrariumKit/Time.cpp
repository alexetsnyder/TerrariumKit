#include "Time.h"

namespace SysTK
{
	std::chrono::duration<double> Time::_deltaTime{ 0.0f };
	std::chrono::high_resolution_clock::time_point Time::_lastFrame{ std::chrono::high_resolution_clock::now() };

	void Time::start()
	{
		_deltaTime = std::chrono::duration<double>(0.0);
		_lastFrame = std::chrono::high_resolution_clock::now();
	}

	double Time::deltaTime()
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

