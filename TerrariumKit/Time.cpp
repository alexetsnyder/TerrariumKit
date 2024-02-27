#include "Time.h"

namespace SysTK
{
	double Time::framesPerSecond_{ 60.0 };
	double Time::secPerFrame_{ 1.0 / framesPerSecond_ };
	std::chrono::duration<double> Time::deltaTime_{ 0.0f };
	std::chrono::steady_clock::time_point Time::lastFrame_{ std::chrono::steady_clock::now() };

	void Time::start()
	{
		deltaTime_ = std::chrono::duration<double>(0.0);
		lastFrame_ = std::chrono::steady_clock::now();
	}

	double Time::deltaTime()
	{
		return deltaTime_.count();
	}

	double Time::fixedDeltaTime()
	{
		return secPerFrame_;
	}

	void Time::update()
	{
		std::chrono::steady_clock::time_point currentFrame = std::chrono::steady_clock::now();
		deltaTime_ = currentFrame - lastFrame_;
		lastFrame_ = currentFrame;
	}
}

