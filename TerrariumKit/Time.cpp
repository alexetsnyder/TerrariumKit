#include "Time.h"

namespace SysTK
{
	std::chrono::duration<double> Time::deltaTime_{ 0.0f };
	std::chrono::high_resolution_clock::time_point Time::lastFrame_{ std::chrono::high_resolution_clock::now() };

	void Time::start()
	{
		deltaTime_ = std::chrono::duration<double>(0.0);
		lastFrame_ = std::chrono::high_resolution_clock::now();
	}

	double Time::deltaTime()
	{
		return deltaTime_.count();
	}

	void Time::update()
	{
		std::chrono::high_resolution_clock::time_point currentFrame = std::chrono::high_resolution_clock::now();
		deltaTime_ = currentFrame - lastFrame_;
		lastFrame_ = currentFrame;
	}
}

